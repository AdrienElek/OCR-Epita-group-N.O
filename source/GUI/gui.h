#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "../sanitizer_ocr/sanitizer.h"
#include "../sanitizer_ocr/loader.h"
#include "../Segmentation/segmentation.h"

char *filename = "";
char *text;
gchar *save_filename = "";
GtkBuilder *builder;

void convertbmp_int(BMP *bmp, int *array);
void makeText(struct Tree *tree, int *array, char *text, int *ind, char *separators, int level, int width);
char* launch(char *file);

void ocr_start(GtkButton *button, GtkBuilder *builder);

void text_modified(GtkTextBuffer *buffer2, GtkBuilder *builder);

void clipboard(GtkButton *button, GtkBuilder *builder);

void save_text(GtkButton *button, GtkBuilder *builder);

void save_as_text(GtkButton *button, GtkBuilder *builder);

void edit_change(GtkButton *button, GtkBuilder *builder);

void cursor_change(GtkButton *button, GtkBuilder *builder);

void wrap_change(GtkButton *button, GtkBuilder *builder);

void text_settings(GtkButton *button, GtkBuilder *builder);

void zoom_image(GtkButton *button, GtkBuilder *builder);

void open_file(GtkButton *button, GtkBuilder *builder);

int init_GUI(int argc, char *argv[]);

#endif
