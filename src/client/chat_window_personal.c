/********************************************************
	>Files			:
	>Description	:
	>Author			: zhouzhi
	>Last Modified	: 2018.09.04, 16:28
 ********************************************************/
#include "GUI.h"
#include <time.h>
GtkWidget *main_chat_personal_window = NULL;
GtkWidget *input_text_buffer;
Entity* chatting_with = NULL;
//得到当前时间
char *get_time()
{
    time_t *timep = malloc(sizeof(*timep));
    time(timep);
    char *s = ctime(timep);
    s[strlen(s)-1] = '\0'; //delete '\n'
    return s;
}
//更新对话界面
void update_buffer(GtkTextBuffer *buffer, char *msg, char *username)
{
    //插入文本到缓冲区
    char now_time[50];
    sprintf(now_time, "%s (%s)\n", "user", get_time());
    gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer), now_time, -1);
    gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer), msg, -1);
}
void send_msg_to(GtkWidget *button, Entity* who)
{
    int id = get_friend_local_id(who->nickname);
    GtkTextIter start,end;
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(input_text_buffer),&start,&end);
    //插入文本到缓冲区:
    static char input[500];
    sprintf(input,"%s\n", gtk_text_buffer_get_text(GTK_TEXT_BUFFER(input_text_buffer),&start,&end,FALSE));
    gtk_text_buffer_set_text(input_text_buffer,"",0); 
    post_msg_unicast(who->nickname, input);
    update_buffer(f_msgbufs[id], input, myself->nickname);
}

void cb_close_window()
{
    gtk_widget_destroy(main_chat_personal_window);
    main_chat_personal_window = NULL;
    chatting_with = NULL;
}

void load_chat_personal_window(Entity* who)
{
    int id = get_friend_local_id(who->nickname);
    chatting_with = who;
    get_time();
    // GtkWidget *main_chat_personal_window;
    GtkWidget *main_box;
    GtkWidget *friend_information_box;
    GtkWidget *friend_table;
    GtkWidget *friend_image;
    GtkWidget *friend_name_label;
    GtkWidget *friend_geqian_label;
    //GtkWidget *nothing_label;
    GtkWidget *chat_view_box;
    //GtkWidget *friend_hseparator;
    GtkWidget *chat_hpaned_window;
    GtkWidget *show_msg_scrolled_window;
    GtkWidget *msg_text;
    GtkWidget *chat_text_view;
    GtkWidget *under_tool_box;
    GtkWidget *smile_button;
    GtkWidget *file_button;
    GtkWidget *chat_history_button;
    GtkWidget *send_msg_box;
    GtkWidget *send_msg_button;
    GtkWidget *input_msg_box;
    GtkWidget *input_msg_text;
    GtkWidget *input_msg_scrolled_window;
    GtkWidget *the_last_box;
    //gchar *title = g_strdup_printf("%s", text_id);

    if(main_chat_personal_window){
        gtk_widget_destroy(main_chat_personal_window);
    }

    //=======================创建界面==============================
    main_chat_personal_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW(main_chat_personal_window),600,500);
    gtk_window_set_title(GTK_WINDOW(main_chat_personal_window), who->nickname);
    g_signal_connect(GTK_OBJECT(main_chat_personal_window), "delete_event", G_CALLBACK(cb_close_window), NULL);

    //=======================上层界面==============================
    main_box = gtk_vbox_new(FALSE, 0);
    under_tool_box = gtk_hbox_new(FALSE, 0);
    input_msg_box = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(main_chat_personal_window), main_box);
    friend_information_box = gtk_vbox_new(FALSE, 0);
    //gtk_container_add(GTK_CONTAINER(main_box), friend_information_box);
    gtk_box_pack_start(GTK_BOX(main_box),friend_information_box,FALSE,FALSE,0);
    //gtk_container_add(GTK_CONTAINER(main_chat_personal_window), friend_information_box);

    friend_table = gtk_table_new(5,5,FALSE);
    //gtk_container_add(GTK_CONTAINER(friend_information_box), friend_table);
    gtk_box_pack_start(GTK_BOX(friend_information_box),friend_table,FALSE,FALSE,0);
    char* imgdir[100];
    sprintf(imgdir, "./imgs/avatars/%02d.png", who->avatar_id);
    friend_image = gtk_image_new_from_file (imgdir);
    gtk_table_attach(GTK_TABLE(friend_table),friend_image,1,3,1,5,GTK_FILL,GTK_FILL,0,0);
    
    friend_name_label = gtk_label_new(who->nickname);
    gtk_table_attach(GTK_TABLE(friend_table),friend_name_label,4,5,1,3,GTK_FILL,GTK_FILL,0,0);

    friend_geqian_label = gtk_label_new("MOTTO");
    gtk_table_attach(GTK_TABLE(friend_table),friend_geqian_label,4,5,4,5,GTK_FILL,GTK_FILL,0,0);

    chat_view_box = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_box),chat_view_box,FALSE,FALSE,0);

    show_msg_scrolled_window = gtk_scrolled_window_new(NULL,NULL);                              /*创建滚动窗口构件*/
    gtk_scrolled_window_set_policy(show_msg_scrolled_window, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtk_box_pack_start(GTK_BOX(chat_view_box),show_msg_scrolled_window,TRUE,TRUE,0);
    gtk_widget_show(show_msg_scrolled_window);               /*显示滚动窗口构件*/
    // view_text_buffer = gtk_text_buffer_new(NULL);`
    msg_text = gtk_text_view_new_with_buffer(f_msgbufs[id]);                                                             /*创建文本视图构件*/
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(show_msg_scrolled_window),msg_text);      /*将文本视图构件加入滚动窗口*/
    gtk_widget_set_size_request(show_msg_scrolled_window, 400, 300);
    gtk_text_view_set_editable(msg_text, FALSE);


    //功能栏
    gtk_box_pack_start(GTK_BOX(main_box),under_tool_box,FALSE,FALSE,0);

    smile_button = sungtk_button_new_with_image("./imgs/IMSmallToolbarFace.ico", 0, 0);
    //获得缓冲区开始和结束位置的Iter
    gtk_box_pack_start(GTK_BOX(under_tool_box),smile_button,FALSE,FALSE,5);
    file_button = sungtk_button_new_with_image("./imgs/IMBigToolbarSendFile.ico", 0, 0);
    gtk_box_pack_start(GTK_BOX(under_tool_box),file_button,FALSE,FALSE,5);


    chat_history_button = gtk_button_new_with_label("聊天历史");
    gtk_box_pack_end(GTK_BOX(under_tool_box),chat_history_button,FALSE,FALSE,5);
    g_signal_connect(GTK_OBJECT(chat_history_button), "clicked", G_CALLBACK(history_message_window), f_msgbufs[id]);

	//输入
	send_msg_box = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(main_box),send_msg_box,FALSE,FALSE,0);

    input_msg_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(input_msg_scrolled_window),GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	input_msg_box = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(send_msg_box),input_msg_box,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(input_msg_box),input_msg_scrolled_window ,FALSE,FALSE,0);
	gtk_widget_show(input_msg_scrolled_window);

    input_text_buffer = gtk_text_buffer_new(NULL);
	input_msg_text = gtk_text_view_new_with_buffer(input_text_buffer);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(input_msg_text), GTK_WRAP_WORD);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(input_msg_scrolled_window), input_msg_text);
	gtk_widget_set_size_request(input_msg_scrolled_window, 400, 150);


	the_last_box = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(send_msg_box),the_last_box,FALSE,FALSE,0);
	send_msg_button = gtk_button_new_with_label("  发  送  ");
    gtk_box_pack_end(GTK_BOX(the_last_box),send_msg_button,FALSE,FALSE,0);
    g_signal_connect(GTK_OBJECT(send_msg_button),"clicked",G_CALLBACK(send_msg_to), who);

    gtk_widget_show_all(main_chat_personal_window);

}

void reload_chat_personal_window(){
    if(chatting_with && ~get_friend_local_id(chatting_with)){
        load_chat_personal_window(chatting_with);
    }else if(chatting_with){
        cb_close_window();
    }
}


