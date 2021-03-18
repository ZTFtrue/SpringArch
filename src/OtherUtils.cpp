/* main.cpp
 *
 * Copyright 2021 ztftrue
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include <string>

#include <iostream>

#include <SystemUtils.cpp>

using namespace std;

class OtherUtils
{
public:
    GtkWidget *window;
    string bootstrapPath = "";
    string computerName = "";
    string rootPassword = "";
    string userName = "";
    string userPassword = "";

    inline static int EFI_Flag = 0;
    inline static int INTEL_Flag = 1;
    inline static int ADMIN_Flag = 2;

    inline static bool isEFI = false;
    inline static bool isIntel = true;
    inline static bool isSamePs = true;

    // TODO 先偷个懒
    inline static GtkWidget *computerEntry;
    inline static GtkWidget *userEntry;
    inline static GtkWidget *userPasswdEntry;
    inline static GtkWidget *userRPasswdEntry;
    inline static GtkWidget *adminPasswdEntry;
    inline static GtkWidget *adminRPasswdEntry;
    inline static GtkWidget *adminPasswdRow;
    inline static GtkWidget *adminRPasswdRow;
    inline static GtkWidget *gtkListBox;
    static void signalFileSet(GtkFileChooserButton *widget,
                              gpointer user_data)
    {
        gchar *path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget));
        std::string value(path);
        static_cast<OtherUtils *>(user_data)->bootstrapPath = value;
    }

    static gboolean signalSwitchState(GtkSwitch *widget,
                                      gboolean state,
                                      gpointer user_data)
    {

        int value = *(int *)user_data;
        if (value == EFI_Flag)
        { //EFI
            isEFI = state == 1;
        }
        else if (value == INTEL_Flag)
        { //CPU
            isIntel = state == 1;
        }
        else if (value == ADMIN_Flag)
        {
            isSamePs = state == 1;
            if (state == 1)
            {
                gtk_container_remove(GTK_CONTAINER(gtkListBox), adminPasswdRow);
                gtk_container_remove(GTK_CONTAINER(gtkListBox), adminRPasswdRow);
            }
            else
            {
                gtk_container_add(GTK_CONTAINER(gtkListBox), adminPasswdRow);
                gtk_container_add(GTK_CONTAINER(gtkListBox), adminRPasswdRow);
            }
        }
        return FALSE;
    }

    GtkWidget *showOtherSelect()
    {
        GtkWidget *windowScroll = gtk_scrolled_window_new(NULL, NULL);
        gtkListBox = gtk_list_box_new();
        gtk_list_box_set_selection_mode(GTK_LIST_BOX(gtkListBox), GTK_SELECTION_NONE);
        SystemUtils systemUtils;
        isEFI = systemUtils.checkIsUefi();
        GtkWidget *row;
        {
            row = gtk_list_box_row_new();
            GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
            gtk_container_add(GTK_CONTAINER(row), hbox);

            GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_box_pack_start(GTK_BOX(hbox), vbox, true, true, 0);

            GtkWidget *label1 = gtk_label_new("Welcome");
            GtkWidget *label2 = gtk_label_new("Lincense is LGPL");
            gtk_box_pack_start(GTK_BOX(vbox), label1, true, true, 0);
            gtk_box_pack_start(GTK_BOX(vbox), label2, true, true, 0);

            GtkWidget *button =
                gtk_link_button_new_with_label("https://github.com/ZTFtrue/SpringArch",
                                               "Get source code");

            gtk_box_pack_start(GTK_BOX(hbox), button, false, true, 0);
            gtk_container_add(GTK_CONTAINER(gtkListBox), row);
        }
        {
            row = gtk_list_box_row_new();
            GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
            gtk_container_add(GTK_CONTAINER(row), hbox);

            GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_box_pack_start(GTK_BOX(hbox), vbox, true, true, 0);

            GtkWidget *label1 = gtk_label_new("Select computer boot is EFI");
            GtkWidget *label2 = gtk_label_new("On is EFI, Off is BOIS ");
            gtk_box_pack_start(GTK_BOX(vbox), label1, true, true, 0);
            gtk_box_pack_start(GTK_BOX(vbox), label2, true, true, 0);
            // isEFI
            GtkWidget *gtkSwitch = gtk_switch_new();
            gtk_switch_set_active(GTK_SWITCH(gtkSwitch), isEFI);
            int id = g_signal_connect(G_OBJECT(gtkSwitch), "state-set", G_CALLBACK(&signalSwitchState), (gpointer)&EFI_Flag);
            gtk_box_pack_start(GTK_BOX(hbox), gtkSwitch, false, true, 0);
            gtk_container_add(GTK_CONTAINER(gtkListBox), row);
        }
        {
            row = gtk_list_box_row_new();
            GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
            gtk_container_add(GTK_CONTAINER(row), hbox);

            GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_box_pack_start(GTK_BOX(hbox), vbox, true, true, 0);

            GtkWidget *label1 = gtk_label_new("It is intel cpu of the computer");
            GtkWidget *label2 = gtk_label_new("On is intel, Off is other ");
            gtk_box_pack_start(GTK_BOX(vbox), label1, true, true, 0);
            gtk_box_pack_start(GTK_BOX(vbox), label2, true, true, 0);

            GtkWidget *gtkSwitch = gtk_switch_new();
            gtk_switch_set_active(GTK_SWITCH(gtkSwitch), isIntel);
            int id = g_signal_connect(G_OBJECT(gtkSwitch), "state-set", G_CALLBACK(&signalSwitchState), (gpointer)&INTEL_Flag);
            gtk_box_pack_start(GTK_BOX(hbox), gtkSwitch, false, true, 0);
            gtk_container_add(GTK_CONTAINER(gtkListBox), row);
        }
        {
            row = gtk_list_box_row_new();
            GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
            gtk_container_add(GTK_CONTAINER(row), hbox);

            GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_box_pack_start(GTK_BOX(hbox), vbox, true, true, 0);

            GtkWidget *label1 = gtk_label_new("Select boot start directory");
            GtkWidget *label2 = gtk_label_new("This is must");
            gtk_box_pack_start(GTK_BOX(vbox), label1, true, true, 0);
            gtk_box_pack_start(GTK_BOX(vbox), label2, true, true, 0);

            GtkWidget *fileChooser = gtk_file_chooser_button_new(("Select boot start"),
                                                                 GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
            int id = g_signal_connect(G_OBJECT(fileChooser), "file-set", G_CALLBACK(&signalFileSet), (gpointer)this);
            gtk_box_pack_start(GTK_BOX(hbox), fileChooser, false, true, 0);
            gtk_container_add(GTK_CONTAINER(gtkListBox), row);
        }
        gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(windowScroll),
                                              gtkListBox);
        return windowScroll;
    }
    GtkWidget *showUserMessage()
    {
        GtkWidget *windowScroll = gtk_scrolled_window_new(NULL, NULL);
        gtkListBox = gtk_list_box_new();
        gtk_list_box_set_selection_mode(GTK_LIST_BOX(gtkListBox), GTK_SELECTION_NONE);
        GtkWidget *row;
        {
            row = gtk_list_box_row_new();
            GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
            gtk_container_add(GTK_CONTAINER(row), hbox);

            GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_box_pack_start(GTK_BOX(hbox), vbox, true, true, 0);

            GtkWidget *label1 = gtk_label_new("Input your computer name:");
            gtk_box_pack_start(GTK_BOX(vbox), label1, true, true, 0);

            computerEntry =
                gtk_entry_new();
            gtk_box_pack_start(GTK_BOX(hbox), computerEntry, false, true, 0);
            gtk_container_add(GTK_CONTAINER(gtkListBox), row);
        }
        {
            row = gtk_list_box_row_new();
            GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
            gtk_container_add(GTK_CONTAINER(row), hbox);

            GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_box_pack_start(GTK_BOX(hbox), vbox, true, true, 0);

            GtkWidget *label1 = gtk_label_new("Input your name:");
            gtk_box_pack_start(GTK_BOX(vbox), label1, true, true, 0);

            userEntry =
                gtk_entry_new();
            gtk_box_pack_start(GTK_BOX(hbox), userEntry, false, true, 0);
            gtk_container_add(GTK_CONTAINER(gtkListBox), row);
        }
        {
            row = gtk_list_box_row_new();
            GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
            gtk_container_add(GTK_CONTAINER(row), hbox);

            GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_box_pack_start(GTK_BOX(hbox), vbox, true, true, 0);

            GtkWidget *label1 = gtk_label_new("Input your password:");
            gtk_box_pack_start(GTK_BOX(vbox), label1, true, true, 0);

            userPasswdEntry = gtk_entry_new();
            gtk_entry_set_visibility(GTK_ENTRY(userPasswdEntry), false);

            gtk_box_pack_start(GTK_BOX(hbox), userPasswdEntry, false, true, 0);
            gtk_container_add(GTK_CONTAINER(gtkListBox), row);
        }
        {
            row = gtk_list_box_row_new();
            GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
            gtk_container_add(GTK_CONTAINER(row), hbox);

            GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_box_pack_start(GTK_BOX(hbox), vbox, true, true, 0);

            GtkWidget *label1 = gtk_label_new("Again input your password:");
            GtkWidget *label2 = gtk_label_new("Comfirm your input is correct");
            gtk_box_pack_start(GTK_BOX(vbox), label1, true, true, 0);
            gtk_box_pack_start(GTK_BOX(vbox), label2, true, true, 0);

            userRPasswdEntry = gtk_entry_new();
            gtk_entry_set_visibility(GTK_ENTRY(userRPasswdEntry), false);

            gtk_box_pack_start(GTK_BOX(hbox), userRPasswdEntry, false, true, 0);
            gtk_container_add(GTK_CONTAINER(gtkListBox), row);
        }
        {
            adminPasswdRow = gtk_list_box_row_new();
            g_object_ref(adminPasswdRow);

            GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
            gtk_container_add(GTK_CONTAINER(adminPasswdRow), hbox);

            GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_box_pack_start(GTK_BOX(hbox), vbox, true, true, 0);

            GtkWidget *label1 = gtk_label_new("Input admin password:");
            gtk_box_pack_start(GTK_BOX(vbox), label1, true, true, 0);

            adminPasswdEntry = gtk_entry_new();
            gtk_entry_set_visibility(GTK_ENTRY(adminPasswdEntry), false);

            gtk_box_pack_start(GTK_BOX(hbox), adminPasswdEntry, false, true, 0);
            gtk_container_add(GTK_CONTAINER(gtkListBox), adminPasswdRow);
        }
        {
            adminRPasswdRow = gtk_list_box_row_new();
            g_object_ref(adminRPasswdRow);
            GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
            gtk_container_add(GTK_CONTAINER(adminRPasswdRow), hbox);

            GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_box_pack_start(GTK_BOX(hbox), vbox, true, true, 0);

            GtkWidget *label1 = gtk_label_new("Again input admin password:");
            GtkWidget *label2 = gtk_label_new("Comfirm your input is correct");
            gtk_box_pack_start(GTK_BOX(vbox), label1, true, true, 0);
            gtk_box_pack_start(GTK_BOX(vbox), label2, true, true, 0);

            adminRPasswdEntry = gtk_entry_new();
            gtk_entry_set_visibility(GTK_ENTRY(adminRPasswdEntry), false);
            gtk_box_pack_start(GTK_BOX(hbox), adminRPasswdEntry, false, true, 0);
            gtk_container_add(GTK_CONTAINER(gtkListBox), adminRPasswdRow);
        }
        {
            row = gtk_list_box_row_new();
            GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
            gtk_container_add(GTK_CONTAINER(row), hbox);

            GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_box_pack_start(GTK_BOX(hbox), vbox, true, true, 0);

            GtkWidget *label1 = gtk_label_new("Use same password for admin");
            gtk_box_pack_start(GTK_BOX(vbox), label1, true, true, 0);

            GtkWidget *gtkSwitch = gtk_switch_new();
            int id = g_signal_connect(G_OBJECT(gtkSwitch), "state-set", G_CALLBACK(&signalSwitchState), (gpointer)&ADMIN_Flag);
            gtk_box_pack_start(GTK_BOX(hbox), gtkSwitch, false, true, 0);
            gtk_container_add(GTK_CONTAINER(gtkListBox), row);
        }
        gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(windowScroll),
                                              gtkListBox);
        return windowScroll;
    }
    string getText()
    {
        computerName = gtk_entry_get_text(GTK_ENTRY(computerEntry));
        userName = gtk_entry_get_text(GTK_ENTRY(userEntry));
        userPassword = gtk_entry_get_text(GTK_ENTRY(userPasswdEntry));
        string userP(gtk_entry_get_text(GTK_ENTRY(userRPasswdEntry)));
        rootPassword = gtk_entry_get_text(GTK_ENTRY(adminPasswdEntry));
        string rootP(gtk_entry_get_text(GTK_ENTRY(adminRPasswdEntry)));
        if (userName.empty())
        {
            return "User name is empty";
        }
        if (userPassword.empty() || userPassword != userP)
        {
            return "User passwords are not equal or empty";
        }
        if (!isSamePs)
        {
            if (rootPassword.empty() || rootPassword != rootP)
            {
                return "Root password not equal or empty";
            }
        }
        else
        {
            rootPassword = userPassword;
        }
        return "";
    }
};
