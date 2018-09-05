/*************************************************************************
	> File Name: settings.c
	> Author: 
	> Mail: 
	> Created Time: Wed 05 Sep 2018 10:45:59 AM CST
 ************************************************************************/
#include "GUI.h"

void toggle_statusbar(GtkWidget *widget,gpointer statusbar)
{
    if(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))){
        gtk_widget_show(statusbar);
    }else{
        gtk_widget_hide(statusbar);
    }
}

void  load_setting_windows()
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *menubar;
    GtkWidget *statusbar;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   // gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window),250,200);
    gtk_window_set_title(GTK_WINDOW(window),"friends_groups_settings statusbar");
 //===========主分割======
    vbox = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window),vbox);

    menubar = gtk_menu_bar_new();
    
    GtkWidget *menu_friend_groups_settings = gtk_menu_new();
    GtkWidget *friends_groups_settings = gtk_menu_item_new_with_label("好友/群设置");
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(friends_groups_settings),menu_friend_groups_settings);
    GtkWidget *add_new_friend = gtk_menu_item_new_with_label("添加好友");
        gtk_menu_shell_append(GTK_MENU_SHELL(menu_friend_groups_settings),add_new_friend);
    GtkWidget *delete_friend = gtk_menu_item_new_with_label("删除好友");
        gtk_menu_shell_append(GTK_MENU_SHELL(menu_friend_groups_settings),delete_friend);
    GtkWidget *add_new_group = gtk_menu_item_new_with_label("添加群");
        gtk_menu_shell_append(GTK_MENU_SHELL(menu_friend_groups_settings),add_new_group);
    GtkWidget *delete_group = gtk_menu_item_new_with_label("删除群");
        gtk_menu_shell_append(GTK_MENU_SHELL(menu_friend_groups_settings),delete_group);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar),friends_groups_settings);

    GtkWidget *menu_change_interface = gtk_menu_new();
    GtkWidget *change_interface = gtk_menu_item_new_with_label("修改界面");
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(change_interface),menu_change_interface);
    GtkWidget *change_bg_img = gtk_menu_item_new_with_label("修改背景图片");
        gtk_menu_shell_append(GTK_MENU_SHELL(menu_change_interface),change_bg_img);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar),change_interface);

    GtkWidget *menu_change_personal_info=gtk_menu_new();
    GtkWidget *change_personal_info = gtk_menu_item_new_with_label("修改个人信息");
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(change_personal_info),menu_change_personal_info);
    GtkWidget *change_avatar = gtk_menu_item_new_with_label("修改头像");
        gtk_menu_shell_append(GTK_MENU_SHELL(menu_change_personal_info),change_avatar);
    GtkWidget *change_nickname = gtk_menu_item_new_with_label("修改昵称");
        gtk_menu_shell_append(GTK_MENU_SHELL(menu_change_personal_info),change_nickname);
     GtkWidget *change_motto = gtk_menu_item_new_with_label("修改签名");
        gtk_menu_shell_append(GTK_MENU_SHELL(menu_change_personal_info),change_motto);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar),change_personal_info);
   
    GtkWidget *menu_other = gtk_menu_new();
    GtkWidget *other = gtk_menu_item_new_with_label("其他");
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(other),menu_other);
    GtkWidget *about= gtk_menu_item_new_with_label("关于");
        gtk_menu_shell_append(GTK_MENU_SHELL(menu_other),about);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar),other);
    
    
    
    gtk_box_pack_start(GTK_BOX(vbox),menubar,FALSE,FALSE,0);
    
    statusbar = gtk_statusbar_new();
    gtk_box_pack_end(GTK_BOX(vbox),statusbar,FALSE,TRUE,1);

    g_signal_connect_swapped(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);

    g_signal_connect(G_OBJECT(add_new_friend),"activate",G_CALLBACK(toggle_statusbar),statusbar);
    
    gtk_widget_show_all(window);


}