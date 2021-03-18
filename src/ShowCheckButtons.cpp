#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <gtk/gtk.h>
#include <iostream>
#include <string>
#include <map>
using namespace std;

class ShowCheckButtons
{
private:
    std::map<string, string> datas;

public:
    std::map<string, string> results;
    static gboolean signalCheckButton(GtkToggleButton *togglebutton, gpointer p)
    {
        auto pointer = static_cast<ShowCheckButtons *>(p);
        const gchar *a = gtk_button_get_label(GTK_BUTTON(togglebutton));
        string value(a);
        std::map<string, string>::iterator it;

        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton)))
        {
            it = pointer->datas.find(value);
            if (it != pointer->datas.end())
            {
                pointer->results.insert(std::pair<string, string>(value, it->second));
            }
        }
        else
        {
            it = pointer->datas.find(value);
            if (it != pointer->datas.end())
            {
                pointer->results.erase(it);
            }
        }
        return TRUE;
    }
    /**
     * std::map<label, value>
     * */
    GtkWidget *showCheckButton(std::map<string, string> datas)
    {
        this->datas = datas;
        GtkWidget *gtkListBox;
        gtkListBox = gtk_list_box_new();
        // GTK_SELECTION_SINGLE
        gtk_list_box_set_selection_mode(GTK_LIST_BOX(gtkListBox), GTK_SELECTION_NONE);

        for (std::map<std::string, std::string>::iterator it = datas.begin(); it != datas.end(); ++it)
        {

            string label = it->first;
            string value = it->second;
            GtkWidget *row = gtk_list_box_row_new();
            GtkWidget *checkButton = gtk_check_button_new_with_label(label.c_str());

            gtk_container_add(GTK_CONTAINER(row), checkButton);
            gtk_container_add(GTK_CONTAINER(gtkListBox), row);

            int id = g_signal_connect(G_OBJECT(checkButton), "toggled", G_CALLBACK(&signalCheckButton), (gpointer)this);
            // g_signal_handler_disconnect
        }
        return gtkListBox;
    }
};
