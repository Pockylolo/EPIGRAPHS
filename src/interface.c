#include <gtk/gtk.h>
#include "main_process.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "list.h"
#include <stdio.h>
#include <gdk/gdk.h>
#include <cairo.h>
#include <unistd.h>
#include "graph_display.h"
#include "transform.h"


// Structure of the graphical user interface.
typedef struct UserInterface
{
    GtkWindow* window;                      // Main window
    GtkFileChooserButton* loaded_file;      // File chooser --> image loaded
    GtkButton* save_file;                   // Button to save the tree/graph
    GtkEntry* file_name;                    // Name of the saved tree/graph
    GtkImage* image_main;                   // Where the graph is printed
    GtkButton* validate;                    // Update the current graph
    GtkEntry* nb_nodes;
    GtkEntry* text_zone;
    GtkButton* dfs;                         // DFS exercise
    GtkLabel* instructions;
    GtkLabel* answer;                       // Errors are also printed here
    GtkButton* ok;                          // User valid the answer
    GtkButton* reset;
    GtkRadioButton* prefix;
    GtkRadioButton* suffix;
    GtkCheckButton* oriented;
    GtkButton* save;

    //Label (CSS goal)
    GtkLabel* download;
    GtkLabel* graph_part;
    GtkLabel* exercises;
    GtkLabel* save_image;
    GtkLabel* saves_access;

    GtkLayout* options;
} UI;

struct graph *G;

int is_dfs = 0;

////////////////////////////////////////////////////////////////////////////////
//                              FUNCTIONS                                     //
////////////////////////////////////////////////////////////////////////////////

void error_format(UI *ui)
{
    gtk_label_set_label(ui->answer, "Respect the edges format please !!");
    return;
}

void set_reset(GtkButton* button, gpointer user_data)
{
    UI *ui = user_data;
    button = button;

    gtk_image_set_from_file(ui->image_main, "images/background.png");
    gtk_widget_set_sensitive((GtkWidget*) ui->validate, TRUE);

    gtk_widget_set_sensitive((GtkWidget*) ui->save_file, TRUE);
    return;
}

void set_graph(GtkButton *button, gpointer user_data)
{
    UI *ui = user_data;
    button = button;

    //Number of nodes :
    char *entry = malloc(300 * sizeof(char));
    entry = (char *)gtk_entry_get_text(ui->nb_nodes);
    if (entry[1] != '\0') //if nb of nodes > 9
        return;

    //The user edges :
    char *user_edges = malloc(300*sizeof(char));
    user_edges = (char *)gtk_entry_get_text(ui->text_zone);

    char order_char = entry[0];
    int i = 0;
    struct list *edges = list_init();
    if (order_char >= '0' && order_char <= '8')
    {
        int order = order_char - '0';

        while(user_edges[i] != '\0')
        {
            if (user_edges[i] == ' ')
            {
                i++;
                continue;
            }
            struct edge *e = new_edge(0, 0);

            if (user_edges[i] >= '0' && user_edges[i] <= '7')
            {
                int node = user_edges[i] - '0';

                if (node < order)
                    e->src = node;
                else
                    return error_format(ui);
            }
            else
                return error_format(ui);

            // we have already a src node

            i++;
            if (user_edges[i] == '-')
            {
                i++;
                if (user_edges[i] >= '0' && user_edges[i] <= '7')
                {
                    int node = user_edges[i] - '0';

                    if (node < order)
                        e->dest = node;

                    else
                        return error_format(ui);
                }
                else
                    return error_format(ui);
            }
            else
                return error_format(ui);
            i++;

            if ( !gtk_toggle_button_get_active((GtkToggleButton*)((GtkWidget *) ui->oriented)) && is_in_list_edges(0, edges, e) == 1)
            {
                gtk_label_set_label(ui->answer, "Warning ! It seems that you write a dooble same edge...");
                continue;
            }

            if (e->src == e->dest)
                continue;

            push_list_front(edges, (void *)e);
        }

        if (gtk_toggle_button_get_active((GtkToggleButton*)((GtkWidget *) ui->oriented)))
            G = main_process_graph(1, order, edges);
        else
            G = main_process_graph(0, order, edges);
        gtk_image_set_from_file(ui->image_main, "images/graph_nodes.png");
    }
}

void set_DFS(GtkButton *button, gpointer user_data)
{
    UI *ui = user_data;
    button = button;
    is_dfs = 1;

    gtk_widget_set_sensitive((GtkWidget*)ui->validate, FALSE);
    gtk_label_set_label(ui->instructions, "Depth-first search (DFS) exercise:\n(Reminder: the idea of the DFS is to start at the starting node (in our case: zero) and explores each edge as far as possible before go back.) You must write the list of the nodes encountered at the prefix order when you apply a DFS on the graph below. Format: 0, 2, 3, 6... Press -OK- to validate your answer.");

    gtk_entry_set_text(ui->text_zone, "");

}

void set_image(UI *ui)
{
    gtk_image_set_from_file(ui->image_main, "images/graph_nodes.png");
    return;
}

void set_ok(GtkButton *button, gpointer user_data)
{
    if (is_dfs == 0)
        return;

    UI *ui = user_data;
    button = button;

    //List user
    char *user = malloc(300 * sizeof(char));
    user = (char *)gtk_entry_get_text(ui->text_zone);

    if (strlen(user) == 0)
        return;

    int list_user[G->order];
    int i = 0;
    int nb_nodes_answer=0;
    while(user[i] != '\0')
    {
        if (user[i] >= '0' && user[i] < G->order + '0')
        {
            if (nb_nodes_answer < G->order)
            {
                list_user[nb_nodes_answer] = user[i] - '0';
                nb_nodes_answer++;
                i++;
                continue;
            }

            else
            {
                gtk_label_set_label(ui->answer, "You did not write the good number of nodes in your answer. Each node is encountered during the DFS.");
                return;
            }
        }
        if (user[i] == ' ' || user[i] == ',')
        {
            i++;
            continue;
        }
        gtk_label_set_label(ui->answer, "Respect the DFS list format please !");
        return;
    }

    int *DFS_list;
    if (gtk_toggle_button_get_active((GtkToggleButton *)((GtkWidget *) ui->prefix)))
        DFS_list = DFS_Prefix(G);
    else
        DFS_list = DFS_Suffix(G);

    SDL_Surface* Cpy = Convert("images/graph_nodes.png");
    SDL_UnlockSurface(Cpy);
    IMG_SavePNG(Cpy, "images/colored_graph.png");

    for (int j = 0; j < G->order; j++)
    {
        if (list_user[j] != DFS_list[j])
        {
            ColorNode(list_user[j], "red");
            gtk_label_set_label(ui->answer, "You have a mistake. The DFS stop.");
            gtk_image_set_from_file(ui->image_main, "images/colored_graph.png");
            return;
        }
        ColorNode(list_user[j], "green");
        gtk_image_set_from_file(ui->image_main, "images/colored_graph.png");
    }
    free(DFS_list);
    gtk_label_set_label(ui->answer, "Good game ! It is the correct answer.");
    is_dfs = 0;
    return;
}

void set_unload(GtkButton* button, gpointer user_data)
{
    UI *ui = user_data;

    button = button;

    gtk_widget_set_sensitive((GtkWidget*)ui->save_file,FALSE);
}

void set_save(GtkButton *button, gpointer user_data)
{
    button = button;
    UI *ui = user_data;

    char *entry = malloc(300 * sizeof(char));
    char *path = malloc(300 * sizeof(char));

    entry = (char*)gtk_entry_get_text(ui->file_name);

    char folder[13] ={'T','r','e','e','s','_','_','s','a','v','e','d','/'};

    for(int i=0;i<13;i++)
        path[i] = folder[i];

    int j = 0;
    while (entry[j] !='\0')
    {
        *(path+j+13) = *(entry+j);
        j++;
    }

    char extension[4] = {'.','t','x','t'};
    for(int k =0;k<4;k++)
        *(path+k+j+13) = extension[k];


    *(path+j+17)='\0';

    if(*(entry)!='\0')
    {
        int l = 0;
        while(*(path+l) != '\0')
        {
            printf("%c", *(path+l));
            l++;
        }
        printf("\n");
        save_graph(G, path);    //SAVE THE GRAPH WITH INTO THE FILE ("PATH")
        set_unload(button,ui);
    }

    else
    {
        save_graph(G, "Trees__saved/default.txt");

        set_unload(button,ui);
    }
}

void set_load(GtkButton *button, gpointer user_data)
{
    UI *ui = user_data;

    button = button;

    gtk_widget_set_sensitive((GtkWidget*)ui->save_file,TRUE);

    char *path = malloc(300 * sizeof(char));

    path = g_file_get_path(gtk_file_chooser_get_file((GtkFileChooser*)ui->loaded_file));

    G = create_file(path);   //LOAD THE TEXT FILE WHICH CONTAINS THE GRAPH
    struct list *edges = list_init();
    edges = edges_from_graph(G);      //CONSTRUCT THE LIST OF THE GRAPH'S EDGES
    print_graph(G);
    struct list *p = edges->next;
    while(p != NULL)
    {
        struct edge *e = p->elmt;
        printf("e = %d   %d\n", e->src, e->dest);
        p = p->next;
    }

    display_base_graph(G, edges);
    print_graph(G);
    gtk_image_set_from_file(ui->image_main, "images/graph_nodes.png");

    free(path);
}


////////////////////////////////////////////////////////////////////////////////
//                                 MAIN                                       //
////////////////////////////////////////////////////////////////////////////////


int main (/*int argc, char *argv[]*/)
{
    // Initializes GTK.
    gtk_init(NULL, NULL);


    // Constructs a GtkBuilder instance.
    GtkBuilder* builder = gtk_builder_new();

    // Loads the UI description.
    // (Exits if an error occurs.)
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "src/interface.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }
    // Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "src/interface.glade"));

    //GtkDrawingArea* image_main = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "Image_main"));
    GtkImage* image_main = GTK_IMAGE(gtk_builder_get_object(builder, "Image_main"));
    //Left pannel
    GtkFileChooserButton* loaded_file = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "Loaded_file"));
    GtkButton* save_file = GTK_BUTTON(gtk_builder_get_object(builder, "Save_file"));
    GtkEntry* file_name = GTK_ENTRY(gtk_builder_get_object(builder, "File_name"));
    GtkButton* validate = GTK_BUTTON(gtk_builder_get_object(builder, "Validate"));
    GtkEntry* nb_nodes = GTK_ENTRY(gtk_builder_get_object(builder, "Nb_nodes"));
    GtkEntry* text_zone = GTK_ENTRY(gtk_builder_get_object(builder, "Text_zone"));
    GtkButton* dfs = GTK_BUTTON(gtk_builder_get_object(builder, "DFS"));
    GtkLabel* answer = GTK_LABEL(gtk_builder_get_object(builder, "Answer"));
    GtkLabel* instructions = GTK_LABEL(gtk_builder_get_object(builder, "Instructions"));
    GtkLabel* download = GTK_LABEL(gtk_builder_get_object(builder, "Download"));
    GtkLabel* graph_part = GTK_LABEL(gtk_builder_get_object(builder, "Graph_part"));
    GtkLabel* exercises = GTK_LABEL(gtk_builder_get_object(builder, "Exercises"));
    GtkLabel* save_image = GTK_LABEL(gtk_builder_get_object(builder, "Save_image"));
    GtkLabel* saves_access = GTK_LABEL(gtk_builder_get_object(builder, "Saves_access"));
    GtkButton* ok = GTK_BUTTON(gtk_builder_get_object(builder, "Ok"));
    GtkButton* reset = GTK_BUTTON(gtk_builder_get_object(builder, "Reset"));

    GtkLayout* options = GTK_LAYOUT(gtk_builder_get_object(builder, "Options"));

    GtkRadioButton* prefix = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "Prefix"));
 GtkRadioButton* suffix = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "Suffix"));
 GtkCheckButton* oriented = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Oriented"));
 GtkButton* save = GTK_BUTTON(gtk_builder_get_object(builder, "Save"));

    // Creates the "UserInterface" structure.
    UI ui =
    {

        //Left pannel
        .loaded_file = loaded_file,
        .save_file = save_file,
        .file_name = file_name,
        .validate = validate,
        //Right pannel 
        //.area = area,
        .image_main = image_main,
        .nb_nodes = nb_nodes,
        .text_zone = text_zone,
        .dfs = dfs,
        .instructions = instructions,
        .answer = answer,
        .ok = ok,
        .reset = reset,

        //Label
        .download = download,
        .graph_part = graph_part,
        .exercises = exercises,
        .save_image = save_image,
        .saves_access = saves_access,

        .options = options,

        .oriented = oriented,
        .prefix = prefix,
        .suffix = suffix,
        .save = save,
    };

    //CSS
    gtk_widget_set_name((GtkWidget*)image_main, "image_main");
    gtk_widget_set_name((GtkWidget*)loaded_file, "loaded_file");
    gtk_widget_set_name((GtkWidget*)save_file, "save_file");
    gtk_widget_set_name((GtkWidget*)file_name, "file_name");
    gtk_widget_set_name((GtkWidget*)validate, "validate");
    gtk_widget_set_name((GtkWidget*)nb_nodes, "nb_nodes");
    gtk_widget_set_name((GtkWidget*)text_zone, "text_zone");
    gtk_widget_set_name((GtkWidget*)dfs, "dfs");
    gtk_widget_set_name((GtkWidget*)instructions, "instructions");
    gtk_widget_set_name((GtkWidget*)answer, "answer");
    gtk_widget_set_name((GtkWidget*)download, "download");
    gtk_widget_set_name((GtkWidget*)graph_part, "graph_part");
    gtk_widget_set_name((GtkWidget*)exercises, "exercises");
    gtk_widget_set_name((GtkWidget*)save_image, "save_image");
    gtk_widget_set_name((GtkWidget*)options, "options");
    gtk_widget_set_name((GtkWidget*)saves_access, "saves_access");
    gtk_widget_set_name((GtkWidget*)ok, "ok");
    gtk_widget_set_name((GtkWidget*)reset, "reset");
    gtk_widget_set_name((GtkWidget*)save, "save");
    gtk_widget_set_name((GtkWidget*)oriented, "oriented");
    gtk_widget_set_name((GtkWidget*)prefix, "prefix");
    gtk_widget_set_name((GtkWidget*)suffix, "suffix");

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "src/theme.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(cssProvider),
            GTK_STYLE_PROVIDER_PRIORITY_USER);


    gtk_widget_set_sensitive((GtkWidget*)ui.save_file,FALSE);

    // Connects event handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(loaded_file, "file_set", G_CALLBACK(set_load), &ui);
    g_signal_connect(validate, "clicked", G_CALLBACK(set_graph), &ui);
    g_signal_connect(dfs, "clicked", G_CALLBACK(set_DFS), &ui);
    g_signal_connect(ok, "clicked", G_CALLBACK(set_ok), &ui);
    g_signal_connect(reset, "clicked", G_CALLBACK(set_reset), &ui);
    g_signal_connect(save_file, "clicked", G_CALLBACK(set_save), &ui);

    g_signal_connect(save, "clicked", G_CALLBACK(set_save), &ui);

    gtk_widget_show_all((GtkWidget*)window);

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
