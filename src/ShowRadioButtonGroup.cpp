#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <map>
#include <memory>
#include <gtk/gtk.h>
#include <iostream>
#include <string>

using namespace std;

class ShowRadioButtonGroup
{

private:
    // label value
    std::map<string, string> datas;

public:
    std::map<string, string> results;
    static gboolean signalRadioButton(GtkToggleButton *togglebutton, gpointer p)
    {
        auto pointer = static_cast<ShowRadioButtonGroup *>(p);
        const gchar *a = gtk_button_get_label(GTK_BUTTON(togglebutton));
        std::string value(a);
        std::map<string, string>::iterator it;
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton)))
        {
            it = pointer->datas.find(value);
            if (it != pointer->datas.end())
            {
                //Only one select
                pointer->results.empty();
                pointer->results.insert(std::pair<string, string>(value, it->second));
            }
        }
        return TRUE;
    }
    GtkWidget *showRadioButtonGroup(std::map<string, string> datas)
    {
        this->datas = datas;
        GtkWidget *gtkListBox;

        gtkListBox = gtk_list_box_new();
        // GTK_SELECTION_SINGLE
        gtk_list_box_set_selection_mode(GTK_LIST_BOX(gtkListBox), GTK_SELECTION_NONE);
        GtkWidget *radioButton = NULL;
        // make default un selecte anyone.
        radioButton = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radioButton), "");
        g_object_ref(radioButton);
        for (std::map<std::string, std::string>::iterator it = this->datas.begin(); it != this->datas.end(); ++it)
        {
            string label = it->first;
            string value = it->second;
            GtkWidget *row = gtk_list_box_row_new();
            radioButton = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radioButton), label.c_str());
            g_object_ref(radioButton);

            gtk_container_add(GTK_CONTAINER(row), radioButton);
            gtk_container_add(GTK_CONTAINER(gtkListBox), row);
            int id = g_signal_connect(G_OBJECT(radioButton), "toggled", G_CALLBACK(&signalRadioButton),
                                      (gpointer)this);
            // g_signal_handler_disconnect
        }

        return gtkListBox;
    }
};
