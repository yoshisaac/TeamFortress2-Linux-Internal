class Surface {
public:
  void set_rgba(int r, int g, int b, int a) {
    void **vtable = *(void ***)this;

    void (*set_rgba_fn)(void*, int, int, int, int) = (void (*)(void*, int, int, int, int))vtable[10];

    set_rgba_fn(this, r, g, b, a);
  }

  void draw_line(int x1, int y1, int x2, int y2) {
    void **vtable = *(void ***)this;

    void (*draw_line_fn)(void*, int, int, int, int) = (void (*)(void*, int, int, int, int))vtable[15];

    draw_line_fn(this, x1, y1, x2, y2);
  }

  unsigned long text_create_font(void) {
    void **vtable = *(void ***)this;

    unsigned long (*text_create_font_fn)(void*) = (unsigned long (*)(void*))vtable[66];

    return text_create_font_fn(this);
  }

  char text_set_font_glyph_set(unsigned long font, const char *name, int tall, int weight, int blur, int scanlines, int flags) {
    void **vtable = *(void ***)this;

    char (*text_set_font_glyph_set_fn)(void*, unsigned long, const char*, int, int, int, int, int, int, int) =
      (char (*)(void*, unsigned long, const char*, int, int, int, int, int, int, int))vtable[67];

    return text_set_font_glyph_set_fn(this, font, name, tall, weight, blur, scanlines, flags, 0, 0);
  }

  void draw_set_text_font(unsigned long font) {
    void **vtable = *(void ***)this;

    void (*draw_set_text_font_fn)(void*, unsigned long) = (void (*)(void*, unsigned long))vtable[17];

    draw_set_text_font_fn(this, font);
  }

  void draw_set_text_color(int r, int g, int b, int a) {
    void **vtable = *(void ***)this;

    void (*draw_set_text_color_fn)(void*, int, int, int, int) = (void (*)(void*, int, int, int, int))vtable[18];

    draw_set_text_color_fn(this, r, g, b, a);
  }

  void draw_set_text_pos(unsigned int x, unsigned int y) {
    void **vtable = *(void ***)this;

    void (*draw_set_text_pos_fn)(void*, unsigned int, unsigned int) = (void (*)(void*, unsigned int, unsigned int))vtable[20];

    draw_set_text_pos_fn(this, x, y);
  }

  void draw_print_text(const wchar_t *text, int text_len) {
    void **vtable = *(void ***)this;

    void (*draw_print_text_fn)(void*, const wchar_t*, int, int) = (void (*)(void*, const wchar_t*, int, int))vtable[22];

    draw_print_text_fn(this, text, text_len, 0);
  }


};

static inline Surface* surface;
