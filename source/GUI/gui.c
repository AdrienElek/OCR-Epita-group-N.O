/*
GUI OCR
Author: Nicolas LE LAN
*/

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "../ocr.c"

char *filename = "";
char *text;
gchar *save_filename = "";
GtkBuilder *builder;

void ocr_start(GtkButton *button, GtkBuilder *builder)
{
 
  // Desactivation de la fenêtre principale et activation du chargement
  gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "main_window")), FALSE);
  GtkWidget* loading_window = GTK_WIDGET(gtk_builder_get_object(builder, "loading_window"));
  gtk_widget_show(loading_window);

  // Lancement de l'OCR
  
  /*Launch créé le texte*/
    char * texte = launch(filename);
    printf("%s", text);
    free(text);

  //Fin de l'OCR (Texte de résultat: text)
  
  GtkTextBuffer * buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(gtk_builder_get_object(builder, "zone_texte")));
  gtk_text_buffer_set_text(buffer, texte, -1);
  
}

void text_modified(GtkTextBuffer *buffer2, GtkBuilder *builder)
{
  //Fin OCR
  GtkWidget* loading_window = GTK_WIDGET(gtk_builder_get_object(builder, "loading_window"));
  gtk_widget_hide (loading_window);
  gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "main_window")), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "clipboard_button")), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "save_button")), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "save_as_button")), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "text_box")), TRUE);
  gtk_text_buffer_set_modified ( GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "result_buffer")), FALSE);
  //

  // Selection des bornes du texte
  GtkTextIter start;
  GtkTextIter end;
  GtkTextBuffer * buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(gtk_builder_get_object(builder, "zone_texte")));
  gtk_text_buffer_get_start_iter (buffer, &start);
  gtk_text_buffer_get_end_iter (buffer, &end);

  // Application de la police choisie (tag)
  GtkTextTagTable *table = gtk_text_buffer_get_tag_table (buffer);

  if(gtk_text_tag_table_get_size (table) != 0){
    
    GtkTextTag* tag = gtk_text_tag_table_lookup (table, "font");
    gtk_text_buffer_apply_tag (buffer, tag, &start, &end);
    
  }
}

void clipboard(GtkButton *button, GtkBuilder *builder)
{
  //Copie le texte du buffer dans le presse-papier

  // Selection des bornes du texte
  GtkTextIter start;
  GtkTextIter end;
  GtkTextBuffer * buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(gtk_builder_get_object(builder, "zone_texte")));
  gtk_text_buffer_get_start_iter (buffer, &start);
  gtk_text_buffer_get_end_iter (buffer, &end);
  gtk_text_buffer_select_range (buffer,&start,&end);

  //Copie dans le clipboard
  GtkClipboard *clip = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
  gtk_text_buffer_copy_clipboard(buffer, clip);
  gtk_text_buffer_select_range (buffer,&end,&end);
}

void save_text(GtkButton *button, GtkBuilder *builder)
{

  //Enregistrer le texte du buffer dans un fichier
  
  if (save_filename[0] == '\0'){

    // Creation de la boite de dialogue
  GtkWidget *dialog;
  dialog = gtk_file_chooser_dialog_new ("Enregistrer le texte",
					GTK_WINDOW(GTK_WIDGET(gtk_builder_get_object(builder, "main_window"))),
					GTK_FILE_CHOOSER_ACTION_SAVE,
					"_Cancel", GTK_RESPONSE_CANCEL,
					"_Save", GTK_RESPONSE_ACCEPT,
					NULL);

  //Application de filtres
  GtkFileFilter *  filter2 = gtk_file_filter_new();
  gtk_file_filter_set_name(filter2,"text");
  gtk_file_filter_add_mime_type (filter2,"text/plain");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),filter2);

  GtkFileFilter *  filter3 = gtk_file_filter_new();
  gtk_file_filter_set_name(filter3,"All files (.*)");
  gtk_file_filter_add_pattern(filter3,"*");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),filter3);

  //Mise en place d'une icone de fenetre
  GdkPixbuf * icon = gtk_icon_theme_load_icon (gtk_icon_theme_get_default (),
					       "media-floppy",
					       20,
					       0,
					       NULL);
  gtk_window_set_icon (GTK_WINDOW(dialog),icon);
  

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      //Acces au nom du fichier choisi
      save_filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

      // Interface: Desactivation du bouton Enregistrer + Affichage du chemin
      gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "save_button")), FALSE);
      gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "text_path")),save_filename);
      gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "text_box")), FALSE);

      //Selection du texte
      GtkTextIter start;
      GtkTextIter end;
      GtkTextBuffer * buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(gtk_builder_get_object(builder, "zone_texte")));
      gtk_text_buffer_get_start_iter (buffer, &start);
      gtk_text_buffer_get_end_iter (buffer, &end);
      gtk_text_buffer_set_modified (buffer, FALSE);
      
      gchar* text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);

      // Enregistrement du texte
      g_file_set_contents (save_filename, text, strlen(text), NULL);

      g_free (text);

      gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "text_box")), TRUE);

    }
  
  gtk_widget_destroy (dialog);

  }
  else
    {

      // Pareil que dans le if sans affichqge de la boite de dialogue (chemin deja connu)

      
      gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "save_button")), FALSE);

      gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "text_path")),save_filename);


      gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "text_box")), FALSE);

      GtkTextIter start;
      GtkTextIter end;

      GtkTextBuffer * buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(gtk_builder_get_object(builder, "zone_texte")));
      gtk_text_buffer_get_start_iter (buffer, &start);
      gtk_text_buffer_get_end_iter (buffer, &end);
      gchar* text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);

      g_file_set_contents (save_filename, text, strlen(text), NULL);

      g_free (text);

      gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "text_box")), TRUE);
      gtk_text_buffer_set_modified (buffer, FALSE);
    }
}

void save_as_text(GtkButton *button, GtkBuilder *builder)
{

  // Pareil que pour save_text() mais boite de dialogue toujours affichée
  
  GtkWidget *dialog;
  dialog = gtk_file_chooser_dialog_new ("Enregistrer sous",
					GTK_WINDOW(GTK_WIDGET(gtk_builder_get_object(builder, "main_window"))),
					GTK_FILE_CHOOSER_ACTION_SAVE,
					"_Cancel", GTK_RESPONSE_CANCEL,
					"_Save", GTK_RESPONSE_ACCEPT,
					NULL);

  GtkFileFilter *  filter2 = gtk_file_filter_new();
  gtk_file_filter_set_name(filter2,"text");
  gtk_file_filter_add_mime_type (filter2,"text/plain");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),filter2);

  GtkFileFilter *  filter3 = gtk_file_filter_new();
  gtk_file_filter_set_name(filter3,"All files *");
  gtk_file_filter_add_pattern(filter3,"*");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),filter3);

  GdkPixbuf * icon = gtk_icon_theme_load_icon (gtk_icon_theme_get_default (),
					       "media-floppy",
					       20,
					       0,
					       NULL);
  gtk_window_set_icon (GTK_WINDOW(dialog),icon);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      save_filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

      gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "save_button")), FALSE);

      gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "text_path")),save_filename);


      gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "text_box")), FALSE);

      GtkTextIter start;
      GtkTextIter end;

      GtkTextBuffer * buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(gtk_builder_get_object(builder, "zone_texte")));
      gtk_text_buffer_get_start_iter (buffer, &start);
      gtk_text_buffer_get_end_iter (buffer, &end);
      gtk_text_buffer_set_modified (buffer, FALSE);
      gchar* text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);

      g_file_set_contents (save_filename, text, strlen(text), NULL);

      g_free (text);

      gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "text_box")), TRUE);

    }
  
  gtk_widget_destroy (dialog);
}

void edit_change(GtkButton *button, GtkBuilder *builder)
{
  // Editer le texte du buffer (ON/OFF)
  GtkToggleButton* check = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "edit"));
  GtkTextView* text = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "zone_texte"));
  
  if(gtk_toggle_button_get_active (check))
    {
      gtk_text_view_set_editable (text, TRUE);
    }
  else
    {
      gtk_text_view_set_editable (text, FALSE);
    }
}

void cursor_change(GtkButton *button, GtkBuilder *builder)
{

  // Afficher le curseur dans le buffer (ON/OFF)
  GtkToggleButton* check = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "cursor"));
  GtkTextView* text = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "zone_texte"));
  
  if(gtk_toggle_button_get_active (check))
    {
      gtk_text_view_set_cursor_visible (text, TRUE);
    }
  else
    {
      gtk_text_view_set_cursor_visible (text, FALSE);
    }
  
}

void wrap_change(GtkButton *button, GtkBuilder *builder)
{
  // Retour à la ligne auto (ON/OFF)
  GtkToggleButton* check = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "wrap"));
  GtkTextView* text = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "zone_texte"));
  
  if(gtk_toggle_button_get_active (check))
    {
      gtk_text_view_set_wrap_mode (text, GTK_WRAP_WORD_CHAR);
    }
  else
    {
      gtk_text_view_set_wrap_mode (text, GTK_WRAP_NONE);
    }
}

void text_settings(GtkButton *button, GtkBuilder *builder)
{
  //Changer la police du buffer ( tag )

  // Creation de la boite de dialogue
  GtkWidget *dialog;
  dialog = gtk_font_chooser_dialog_new ("Options", GTK_WINDOW(GTK_WIDGET(gtk_builder_get_object(builder, "main_window"))));
  // Mise en place d'une icone
  GdkPixbuf * icon = gtk_icon_theme_load_icon (gtk_icon_theme_get_default (),
					       "font-x-generic",
					       20,
					       0,
					       NULL);
  
  gtk_window_set_icon (GTK_WINDOW(dialog),icon);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_OK){

    // Selection du texte à changer 
    GtkTextIter start;
    GtkTextIter end;

    GtkTextBuffer * buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(gtk_builder_get_object(builder, "zone_texte")));
    gtk_text_buffer_get_start_iter (buffer, &start);
    gtk_text_buffer_get_end_iter (buffer, &end);
    GtkTextTagTable *table = gtk_text_buffer_get_tag_table (buffer);

    // Acces à la police choisie 
    GtkFontChooser* fontchooser = GTK_FONT_CHOOSER(dialog);
    gchar* value = gtk_font_chooser_get_font (fontchooser);

    if(gtk_text_tag_table_get_size (table) != 0){
      gtk_text_tag_table_remove (table, gtk_text_tag_table_lookup (table, "font"));
    }

    // Creation et mise en place de la police (tag)
    GtkTextTag* tag = gtk_text_buffer_create_tag(buffer, "font", "font", value, NULL);
    gtk_text_buffer_apply_tag (buffer, tag, &start, &end);

  }

  gtk_widget_destroy (dialog);
  
}

void zoom_image(GtkButton *button, GtkBuilder *builder)
{

  // Zoom de l'image avec slider

  // Importation de l'image
  GError *error = NULL;
  GdkPixbuf *pix = gdk_pixbuf_new_from_file (filename, &error);
  if (pix == NULL) {
    g_printerr ("Error loading file: #%d %s\n", error->code, error->message);
    g_error_free (error);
    exit (1);
  }

  int wd = gdk_pixbuf_get_width (pix);
  int ht = gdk_pixbuf_get_height (pix);

  //Acces à la valeur du slider
  gdouble value = gtk_adjustment_get_value (GTK_ADJUSTMENT(gtk_builder_get_object(builder, "slider_scale")));

  //Poucentage -> valeur
  double wds = (double) wd*(value/100);
  double hts = (double) ht*(value/100);

  // Creation d'une nouvelle image avec L x l modifiées
  GdkPixbuf *pix2 = gdk_pixbuf_scale_simple (pix, (int)wds, (int)hts, GDK_INTERP_BILINEAR);

  // Mise en place de l'image
  gtk_image_set_from_pixbuf (GTK_IMAGE(gtk_builder_get_object(builder, "image_filename")), pix2);
}

void open_file(GtkButton *button, GtkBuilder *builder)
{
  // Ouvrir le fichier image

  // Creation de la boite de dialogue
  GtkWidget *dialog;
  dialog = gtk_file_chooser_dialog_new ("Ouvrir une image",
					GTK_WINDOW(GTK_WIDGET(gtk_builder_get_object(builder, "main_window"))),
					GTK_FILE_CHOOSER_ACTION_OPEN,
					"_Cancel", GTK_RESPONSE_CANCEL,
					"_Open", GTK_RESPONSE_ACCEPT,
					NULL);


  // Mise en place du filtre bmp
  GtkFileFilter *  filter = gtk_file_filter_new();
  gtk_file_filter_set_name(filter,"Bitmap (.bmp)");
  gtk_file_filter_add_mime_type(filter,"image/bmp");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),filter);

  // Mise en place d'une icone de fenetre
  GdkPixbuf * icon = gtk_icon_theme_load_icon (gtk_icon_theme_get_default (),
					       "folder-open",
					       20,
					       0,
					       NULL);
  gtk_window_set_icon (GTK_WINDOW(dialog),icon);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      // Acces au chemin du fichier choisi
      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

      // Interface
      gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "ocr_button")), TRUE);
      gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "part_image")), TRUE);

      // Affichage du chemin 
      gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "file_path")),filename);
      // Affichage de l'image
      gtk_image_set_from_file (GTK_IMAGE(gtk_builder_get_object(builder, "image_filename")), filename);
      

      //Default settings
      GtkTextBuffer * buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(gtk_builder_get_object(builder, "zone_texte")));
      gtk_text_buffer_set_text (buffer, "", -1);
      gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "clipboard_button")), FALSE);
      gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "save_button")), FALSE);
      gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "save_as_button")), FALSE);
      gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(builder, "text_box")), FALSE);
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "cursor")), FALSE);
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "edit")), FALSE);
      save_filename = "";
      gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "text_path")),save_filename);

      gtk_adjustment_set_value (GTK_ADJUSTMENT(gtk_builder_get_object(builder, "slider_scale")), 100);

      GtkTextTagTable *table = gtk_text_buffer_get_tag_table (buffer);
      if(gtk_text_tag_table_get_size (table) != 0){
	gtk_text_tag_table_remove (table, gtk_text_tag_table_lookup (table, "font"));
      }
      //
      
    }
  
  gtk_widget_destroy (dialog);
}


// Main function.
int init_GUI(int argc, char *argv[])
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "source/GUI/gui.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.

       //Windows
    GtkWidget* main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
       //Buttons
    GtkWidget* open_button = GTK_WIDGET(gtk_builder_get_object(builder, "open_button"));
    GtkWidget* ocr_button = GTK_WIDGET(gtk_builder_get_object(builder, "ocr_button"));
    GtkWidget* clipboard_button = GTK_WIDGET(gtk_builder_get_object(builder, "clipboard_button"));
    GtkWidget* save_button = GTK_WIDGET(gtk_builder_get_object(builder, "save_button"));
    GtkWidget* save_as_button = GTK_WIDGET(gtk_builder_get_object(builder, "save_as_button"));
    GtkWidget* edit = GTK_WIDGET(gtk_builder_get_object(builder, "edit"));
    GtkWidget* cursor = GTK_WIDGET(gtk_builder_get_object(builder, "cursor"));
    GtkWidget* wrap = GTK_WIDGET(gtk_builder_get_object(builder, "wrap"));
    GtkWidget* button_plus = GTK_WIDGET(gtk_builder_get_object(builder, "button_plus"));
      //Buffer
    GtkTextBuffer* result_buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "result_buffer"));
      //Rqnge
    GtkRange* range = GTK_RANGE(gtk_builder_get_object(builder, "image_scale"));

    // Connects signal handlers.
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(open_button, "clicked", G_CALLBACK(open_file), builder);
    g_signal_connect(ocr_button, "clicked", G_CALLBACK(ocr_start), builder);
    g_signal_connect(clipboard_button, "clicked", G_CALLBACK(clipboard), builder);
    g_signal_connect(save_button, "clicked", G_CALLBACK(save_text), builder);
    g_signal_connect(save_as_button, "clicked", G_CALLBACK(save_as_text), builder);
    g_signal_connect(edit, "clicked", G_CALLBACK(edit_change), builder);
    g_signal_connect(cursor, "clicked", G_CALLBACK(cursor_change), builder);
    g_signal_connect(wrap, "clicked", G_CALLBACK(wrap_change), builder);
    g_signal_connect(result_buffer, "modified-changed", G_CALLBACK(text_modified), builder);
    g_signal_connect(button_plus, "clicked", G_CALLBACK(text_settings), builder);
    g_signal_connect(range, "value-changed", G_CALLBACK(zoom_image), builder);


    gtk_widget_show(main_window);
    
    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
