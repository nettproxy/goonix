#ifndef VIDEO_H
#define VIDEO_H

void video_init(void);
void video_clear_screen(void);
void video_put_char(char c);
void video_erase_char(void);
void video_draw_cursor(int visible);
void video_print_string(const char *str);
void video_redraw_line(int line_start, const char *buffer, int length, int cursor_pos);
int video_get_cursor_pos(void);
void video_set_cursor_pos(int pos);

#endif // VIDEO_H
