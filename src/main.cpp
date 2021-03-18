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

#include <memory>
#include <gtk/gtk.h>
#include <iostream>
#include <string>
#include <vector>

#include <stdlib.h> /* system, NULL, EXIT_FAILURE */
#include <stdexcept>
#include <stdio.h>
#include <list>
#include <iterator>

#include "DiskUtils.cpp"
#include "FileUtils.cpp"
#include "ShowRadioButtonGroup.cpp"
#include "ShowCheckButtons.cpp"
#include "OtherUtils.cpp"
#include "Generateinstall.cpp"

using namespace std;

vector<GtkWidget *> gtkWidgetVec;

list<string> stepStrings;

GtkWidget *window;
int currentPosition = 0;
const int nextStep = 1;
const int backStep = 2;
const int onlyGenerateFlag = 3;
const int startInstallFlag = 4;

OtherUtils otherUtils;
ShowRadioButtonGroup timeZones;
ShowRadioButtonGroup keyborads;
ShowCheckButtons sourceCheck;
ShowCheckButtons languageCheck;
ShowCheckButtons softwareCheck;
ShowRadioButtonGroup displayManager;
DiskUtils diskUtils;
Generateinstall generateinstall;

list<string> mapToList(std::map<string, string> datas)
{
    list<string> values;
    for (std::map<std::string, std::string>::iterator it = datas.begin(); it != datas.end(); ++it)
    {
        string value = it->second;
        values.push_back(value);
    }
    return values;
}

string mapToString(std::map<string, string> datas)
{
    return datas.empty() ? "" : datas.begin()->second;
}
void showDialog(string message)
{
    GtkWidget *dialog = NULL;
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, message.c_str());
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
string generateShFile()
{
    if (diskUtils.results.empty())
    {
        showDialog("No select any disk volume");
        return "";
    }
    string r = otherUtils.getText();
    if (!r.empty())
    {
        showDialog(r);
        return "";
    }
    return generateinstall.generate(otherUtils.isEFI, otherUtils.isIntel, mapToString(timeZones.results),
                                    mapToString(keyborads.results),
                                    mapToList(sourceCheck.results),
                                    mapToList(languageCheck.results),
                                    mapToList(softwareCheck.results),
                                    mapToString(displayManager.results), diskUtils.results, diskUtils.recordDisk, otherUtils.computerName, otherUtils.rootPassword, otherUtils.userName, otherUtils.userPassword, otherUtils.bootstrapPath);
}
void installButtonClick(GtkButton *button,
                        gpointer data)
{
    int value = *(int *)data;
    if (onlyGenerateFlag == value) // Only generate
    {
        string p = generateShFile();
        if (p.empty())
        {
            showDialog("Generate file error");
        }
        else
        {
            showDialog("Generate file finish");
        }

        // otherUtils.isEFI
        // otherUtils.isIntel
        // timeZones.results
        // keyborads.results
        // sourceCheck.results
        // languageCheck.results
        // softwareCheck.results
        // displayManager.results
        // diskUtils
    }
    else if (startInstallFlag == value)
    {
        string path = generateShFile();
        // int execl(const char *pathname, const char *arg, ...
        // maybe it can input password.
        if (!path.empty())
        {
            int ret = execl(path.c_str(), NULL);
            if (ret == -1)
            {
                showDialog("Insatll error");
            }
        }
        else
        {
            showDialog("Generate file error");
        }
    }
}
GtkWidget *showInstalling()
{
    GtkWidget *windowScroll = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *gtkListBox;
    gtkListBox = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(gtkListBox), GTK_SELECTION_NONE);
    GtkWidget *row;
    {
        row = gtk_list_box_row_new();
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
        gtk_container_add(GTK_CONTAINER(row), hbox);

        GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_pack_start(GTK_BOX(hbox), vbox, true, true, 0);

        GtkWidget *label1 = gtk_label_new("Only generate install file in bootstrap directory.");
        GtkWidget *label2 = gtk_label_new("The file is \"install.sh\"");
        gtk_box_pack_start(GTK_BOX(vbox), label1, true, true, 0);
        gtk_box_pack_start(GTK_BOX(vbox), label2, true, true, 0);

        GtkWidget *button = gtk_button_new_with_label("Generate install.sh");

        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(installButtonClick),
                         (gpointer)&onlyGenerateFlag);
        gtk_box_pack_start(GTK_BOX(hbox), button, false, true, 0);
        gtk_container_add(GTK_CONTAINER(gtkListBox), row);
    }
    {
        row = gtk_list_box_row_new();
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
        gtk_container_add(GTK_CONTAINER(row), hbox);

        GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_pack_start(GTK_BOX(hbox), vbox, true, true, 0);

        GtkWidget *label1 = gtk_label_new("The install file in your bootstrap directory.");
        GtkWidget *label2 = gtk_label_new("Thank you.");
        gtk_box_pack_start(GTK_BOX(vbox), label1, true, true, 0);
        gtk_box_pack_start(GTK_BOX(vbox), label2, true, true, 0);
        // isEFI
        GtkWidget *button = gtk_button_new_with_label("Start install");

        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(installButtonClick),
                         (gpointer)&startInstallFlag);
        gtk_box_pack_start(GTK_BOX(hbox), button, false, true, 0);
        gtk_container_add(GTK_CONTAINER(gtkListBox), row);
    }
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(windowScroll),
                                          gtkListBox);
    return windowScroll;
}

GtkWidget *showTimeZones()
{
    GtkWidget *windowScroll = gtk_scrolled_window_new(NULL, NULL);
    std::map<string, string> datas;
    FileUtils fileUtils;
    fileUtils.getTimeZone(&datas, otherUtils.bootstrapPath);

    GtkWidget *gw = timeZones.showRadioButtonGroup(datas);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(windowScroll), gw);
    return windowScroll;
}

GtkWidget *showKeybords()
{
    std::map<string, string> keymaps;
    GtkWidget *windowScroll = gtk_scrolled_window_new(NULL, NULL);
    FileUtils fileUtils;
    fileUtils.getKeybords(&keymaps, otherUtils.bootstrapPath);

    GtkWidget *gw = keyborads.showRadioButtonGroup(keymaps);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(windowScroll), gw);
    return windowScroll;
}

GtkWidget *showSource()
{
    std::map<string, string> keymaps;
    GtkWidget *windowScroll = gtk_scrolled_window_new(NULL, NULL);
    FileUtils fileUtils;
    std::list<string> datas;
    fileUtils.readFile(&datas, otherUtils.bootstrapPath + "/etc/pacman.d/mirrorlist");
    string lastLabel = "";
    for (std::list<std::string>::iterator it = datas.begin(); it != datas.end(); ++it)
    {
        string value = *it;
        std::size_t found = value.find_last_of("#");
        if (found == string::npos)
        {
            continue;
        }
        else if (found == 1)
        {
            lastLabel = value.substr(2, string::npos);
        }
        else if (found == 0)
        {
            keymaps.insert(std::pair<string, string>(lastLabel + "  " + value, value.substr(1, string::npos)));
        }
    }

    GtkWidget *gw = sourceCheck.showCheckButton(keymaps);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(windowScroll), gw);
    return windowScroll;
}

GtkWidget *showLanguage()
{
    std::map<string, string> keymaps;
    GtkWidget *windowScroll = gtk_scrolled_window_new(NULL, NULL);
    FileUtils fileUtils;
    std::list<string> datas;
    fileUtils.readFile(&datas, otherUtils.bootstrapPath + "/etc/locale.gen");
    for (std::list<std::string>::iterator it = datas.begin(); it != datas.end(); ++it)
    {
        string value = *it;
        if (value != "#" && value.find_first_of(" ") != 1)
        {
            keymaps.insert(std::pair<string, string>(value.substr(1, string::npos), value.substr(1, string::npos)));
        }
    }
    GtkWidget *gw = languageCheck.showCheckButton(keymaps);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(windowScroll), gw);
    return windowScroll;
}

GtkWidget *showSoftware()
{
    std::map<string, string> keymaps;
    GtkWidget *windowScroll = gtk_scrolled_window_new(NULL, NULL);
    FileUtils fileUtils;
    std::list<string> datas;
    fileUtils.readFile(&datas, otherUtils.bootstrapPath + "/software");
    string title;
    for (std::list<std::string>::iterator it = datas.begin(); it != datas.end(); ++it)
    {
        string value = *it;
        if (value.find_first_of("#") == 0)
        {
            title = value.substr(1, string::npos);
        }
        else
        {
            keymaps.insert(std::pair<string, string>(title + "\n  " + value, value));
        }
    }
    GtkWidget *gw = softwareCheck.showCheckButton(keymaps);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(windowScroll), gw);
    return windowScroll;
}
GtkWidget *showStartManager()
{
    std::map<string, string> keymaps;
    GtkWidget *windowScroll = gtk_scrolled_window_new(NULL, NULL);
    // systemctl enable gdm
    keymaps.insert(std::pair<string, string>("gdm", "gdm"));
    keymaps.insert(std::pair<string, string>("sddm", "sddm"));
    GtkWidget *gw = displayManager.showRadioButtonGroup(keymaps);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(windowScroll), gw);
    return windowScroll;
}

void nextAddWidget()
{
    {
        GtkWidget *gtkW = diskUtils.createDiskList(otherUtils.bootstrapPath);
        g_object_ref(gtkW);
        gtkWidgetVec.push_back(gtkW);
    }
    {
        GtkWidget *gtkW = showTimeZones();
        g_object_ref(gtkW);
        gtkWidgetVec.push_back(gtkW);
    }
    {
        GtkWidget *gtkW = showKeybords();
        g_object_ref(gtkW);
        gtkWidgetVec.push_back(gtkW);
    }
    {
        GtkWidget *gtkW = showSource();
        g_object_ref(gtkW);
        gtkWidgetVec.push_back(gtkW);
    }
    {
        GtkWidget *gtkW = showLanguage();
        g_object_ref(gtkW);
        gtkWidgetVec.push_back(gtkW);
    }
    {
        GtkWidget *gtkW = showSoftware();
        g_object_ref(gtkW);
        gtkWidgetVec.push_back(gtkW);
    }
    {
        GtkWidget *gtkW = showStartManager();
        g_object_ref(gtkW);
        gtkWidgetVec.push_back(gtkW);
    }
    {
        GtkWidget *gtkW = otherUtils.showUserMessage();
        g_object_ref(gtkW);
        gtkWidgetVec.push_back(gtkW);
    }
    {
        GtkWidget *gtkW = showInstalling();
        g_object_ref(gtkW);
        gtkWidgetVec.push_back(gtkW);
    }
}
void buttonClick(GtkWidget *wid, gpointer *data)
{

    if (otherUtils.bootstrapPath.empty())
    {
        return;
    }
    int value = *(int *)data;
    if (value == nextStep)
    { // next
        if (gtkWidgetVec.size() == 1)
        {
            nextAddWidget();
        }

        if (currentPosition >= gtkWidgetVec.size() - 1)
        {
            return;
        }
        GtkWidget *gtkWidget = gtkWidgetVec.at(currentPosition);
        gtk_container_remove(GTK_CONTAINER(window), gtkWidget);
        currentPosition = currentPosition + 1;
        GtkWidget *gtkWidgetgtkWidget = gtkWidgetVec.at(currentPosition);
        gtk_container_add(GTK_CONTAINER(window), gtkWidgetgtkWidget);
        gtk_widget_show_all(window);
    }
    else if (value == backStep)
    {
        // back
        if (currentPosition == 0)
        {
            return;
        }
        GtkWidget *gtkWidget = gtkWidgetVec.at(currentPosition);
        gtk_container_remove(GTK_CONTAINER(window), gtkWidget);
        currentPosition = currentPosition - 1;
        gtkWidget = gtkWidgetVec.at(currentPosition);
        gtk_container_add(GTK_CONTAINER(window), gtkWidget);
    }
    std::list<std::string>::iterator it = stepStrings.begin();
    std::advance(it, currentPosition);
    gtk_window_set_title(GTK_WINDOW(window), it->c_str());
}
void addHeadBar()
{

    GtkWidget *headerBar = gtk_header_bar_new();
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *button = NULL;

    button = gtk_button_new_with_label("Back");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(buttonClick),
                     (gpointer)&backStep);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), button);
    button = gtk_button_new_with_label("Next");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(buttonClick),
                     (gpointer)&nextStep);
    gtk_box_pack_start(GTK_BOX(vbox), button, false, true, 0);

    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerBar), TRUE);
    gtk_header_bar_set_title(GTK_HEADER_BAR(headerBar), "SpringArchlinux");

    gtk_header_bar_pack_end(GTK_HEADER_BAR(headerBar), vbox);

    gtk_window_set_titlebar(GTK_WINDOW(window), headerBar);
}

void initTitle()
{
    stepStrings.push_back("Select");
    stepStrings.push_back("Select mount disk");
    stepStrings.push_back("Select zone(single)");
    stepStrings.push_back("Select key board(Non-essential, you can click next to skip)");
    stepStrings.push_back("Select soft source(multiple. Must select)");
    stepStrings.push_back("Select language(multiple, recommend select)");
    stepStrings.push_back("Select software(multiple, you can click next to skip)");
    stepStrings.push_back("Select Display manager(single, you can click next to skip)");
    stepStrings.push_back("Write your message");
    stepStrings.push_back("Installing");
}

int main(int argc, char *argv[])
{
    initTitle();
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    addHeadBar();

    GtkWidget *gtkWidget = otherUtils.showOtherSelect();
    g_object_ref(gtkWidget);
    gtkWidgetVec.push_back(gtkWidget);
    gtk_container_add(GTK_CONTAINER(window), gtkWidget);

    gtk_window_set_default_size(GTK_WINDOW(window), 800, 500);
    gtk_container_set_border_width(GTK_CONTAINER(window), 8);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_widget_realize(window);
    g_signal_connect(window, "destroy", gtk_main_quit, NULL);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
