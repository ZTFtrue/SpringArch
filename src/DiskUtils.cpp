/*
 * DiskUtils.h
 *
 *  Created on: Mar 1, 2021
 *      Author: ztftrue
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <gtk/gtk.h>
#include <map>

#include "FileUtils.cpp"

using namespace std;

class DiskUtils
{
public:
  struct MultiData
  {
    DiskUtils *pointer;
    GtkWidget *user_data;
  };
  // TODO will be replace.
  std::map<string, string> results;
  std::map<string, string> recordDisk;

  static void selectSingal(GtkWidget *dialog,
                           gpointer *user_data)
  {
    //I don't know , why it crash.
    //gtk_combo_box_set_active(GTK_COMBO_BOX(user_data), -1);
  }

  /**
 * 磁盘
 * 路径
 * **/
  static void comBoxSingal(GtkComboBoxText *widget,
                           MultiData *multiData)
  {

    GtkWidget *label = multiData->user_data;

    auto pointer = multiData->pointer;

    string dir(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)));
    string disk(gtk_label_get_text(GTK_LABEL(label)));
    // This can set Id;
    if (dir == "Don't use")
    {
      std::map<string, string>::iterator it;

      it = pointer->results.find(disk);
      if (it != pointer->results.end())
      {
        pointer->results.erase(it);
      }
      return;
    }
    for (std::map<string, string>::iterator it = pointer->results.begin(); it != pointer->results.end(); ++it)
    {
      if (it->first == disk || it->second == dir)
      {
        GtkWidget *dialog = NULL;
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                                        GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Don't repeat select disk or directory");
        g_signal_connect(G_OBJECT(dialog), "response", G_CALLBACK(&selectSingal),
                         (gpointer)widget);
        g_signal_connect(G_OBJECT(dialog), "close", G_CALLBACK(&selectSingal),
                         (gpointer)widget);
        gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
      }
    }
    pointer->results.insert(std::pair<string, string>(disk, dir));
  }
  std::string trim(const std::string &str,
                   const std::string &whitespace = " ")
  {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
      return std::string(); // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
  }

  vector<std::string> splitBySpaces(const std::string &str,
                                    const std::string &fill = " ",
                                    const std::string &whitespace = " ")
  {
    // trim first
    auto result = trim(str, whitespace);

    vector<std::string> strs;
    while (result != "")
    {
      auto beginSpace = 0;
      const auto endSpace = result.find_first_of(whitespace);
      const auto range = endSpace - beginSpace;
      std::string str2 = result.substr(0, range);
      strs.push_back(str2);
      result = result.substr(endSpace, std::string::npos);
      result = trim(result, whitespace);
      if (strs.size() == 3)
      {
        strs.push_back(result);
        break;
      }
    }
    return strs;
  }
  vector<string> getDisksBlocks()
  {
    string line;
    vector<string> devs;
    ifstream myfile("/proc/partitions");
    if (myfile.is_open())
    {
      int c = 0;
      string lastDisk = "";
      while (getline(myfile, line))
      {
        int N = line.length();
        if (c == 0 || N == 0)
        {
          ++c;
          continue;
        }
        vector<string> strs = splitBySpaces(line);
        if (strs.at(1) != "0")
        {
          devs.push_back(strs.at(3));
          recordDisk.insert(std::pair<string, string>(strs.at(3), lastDisk));
        }
        else
        {
          lastDisk = strs.at(3);
        }
      }
      myfile.close();
    }
    else
      cout << "Unable to open file";
    return devs;
  }

  GtkWidget *createDiskList(string bootstrapPath)
  {
    GtkWidget *windowScroll = gtk_scrolled_window_new(NULL, NULL);

    GtkWidget *gtkListBox;

    DiskUtils disktuils;
    vector<string> disks = disktuils.getDisksBlocks();

    gtkListBox = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(gtkListBox), GTK_SELECTION_NONE);
    vector<string> rootDir;
    for (auto &p : fs::directory_iterator(bootstrapPath))
    {
      fs::path dir = p.path();
      if (fs::is_directory(p.symlink_status()))
      {
        rootDir.push_back(dir.string().substr(bootstrapPath.size(), string::npos));
      }
    }
    rootDir.push_back("/");
    rootDir.push_back("Don't use");
    g_signal_new("select-error",
                 G_TYPE_OBJECT, G_SIGNAL_RUN_FIRST,
                 0, NULL, NULL,
                 g_cclosure_marshal_VOID__POINTER,
                 G_TYPE_NONE, 1, G_TYPE_POINTER);
    for (int i = 0; i < disks.size(); i++)
    {
      GtkWidget *row = gtk_list_box_row_new();
      GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
      gtk_container_add(GTK_CONTAINER(row), hbox);

      GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
      gtk_box_pack_start(GTK_BOX(hbox), vbox, true, true, 0);

      GtkWidget *label1 = gtk_label_new(disks.at(i).c_str());
      GtkWidget *label2 = gtk_label_new("Select you disk mount directory");
      gtk_box_pack_start(GTK_BOX(vbox), label1, true, true, 0);
      gtk_box_pack_start(GTK_BOX(vbox), label2, true, true, 0);

      GtkWidget *gtkCombo = gtk_combo_box_text_new();

      for (int i = 0; i < rootDir.size(); i++)
      { // 填充gtkCombo
        gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtkCombo), NULL, NULL, rootDir.at(i).c_str());
      }
      g_object_ref(gtkCombo);
      // TODO
      MultiData *multiData=new MultiData;
      multiData->pointer = this;
      multiData->user_data = label1;
      g_signal_connect(G_OBJECT(gtkCombo), "changed", G_CALLBACK(&comBoxSingal),
                       multiData);
      // g_signal_connect(G_OBJECT(gtkCombo), "select-error", G_CALLBACK(&selectSingal), (gpointer)gtkCombo);
      gtk_box_pack_start(GTK_BOX(hbox), gtkCombo, false, true, 0);
      gtk_container_add(GTK_CONTAINER(gtkListBox), row);
    }
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(windowScroll),
                                          gtkListBox);
    return windowScroll;
  }
};
