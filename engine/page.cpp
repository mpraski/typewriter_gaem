//
// Created by marcin on 7/19/19.
//

#include "page.hpp"

namespace {
// Add an underline or strikethrough line to the vertex array
void add_line(sf::VertexArray &vertices,
              float x,
              float width,
              float lineTop,
              const sf::Color &color,
              float offset,
              float thickness) {
  float top{std::floor(lineTop + offset - (thickness / 2) + 0.5f)};
  float bottom{top + std::floor(thickness + 0.5f)};

  vertices.append(sf::Vertex(sf::Vector2f(x, top), color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(sf::Vector2f(x + width, top), color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(sf::Vector2f(x, bottom), color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(sf::Vector2f(x, bottom), color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(sf::Vector2f(x + width, top), color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(sf::Vector2f(x + width, bottom), color, sf::Vector2f(1, 1)));
}

void add_font_background(sf::VertexArray &vertices,
                         sf::Vector2f position,
                         const sf::Glyph &glyph,
                         float italicShear) {
  float padding{1.f};

  float left{glyph.bounds.left - padding};
  float top{glyph.bounds.top - padding};
  float right{glyph.bounds.left + glyph.bounds.width + padding};
  float bottom{glyph.bounds.top + glyph.bounds.height + padding};

  vertices.append(
      sf::Vertex(sf::Vector2f(position.x + left - italicShear * top, position.y + top), sf::Vector2f(0, 0)));
  vertices.append(
      sf::Vertex(sf::Vector2f(position.x + right - italicShear * top, position.y + top),
                 sf::Vector2f(glyph.bounds.width + padding, 0)));
  vertices.append(sf::Vertex(sf::Vector2f(position.x + left - italicShear * bottom, position.y + bottom),
                             sf::Vector2f(0, glyph.bounds.height + padding)));
  vertices.append(sf::Vertex(sf::Vector2f(position.x + left - italicShear * bottom, position.y + bottom),
                             sf::Vector2f(0, glyph.bounds.height + padding)));
  vertices.append(
      sf::Vertex(sf::Vector2f(position.x + right - italicShear * top, position.y + top),
                 sf::Vector2f(glyph.bounds.width + padding, 0)));
  vertices.append(sf::Vertex(sf::Vector2f(position.x + right - italicShear * bottom, position.y + bottom),
                             sf::Vector2f(glyph.bounds.width + padding, glyph.bounds.height + padding)));
}

// Add a glyph quad to the vertex array
void add_glyph_quad(sf::VertexArray &vertices,
                    sf::Vector2f position,
                    const sf::Color &color,
                    const sf::Glyph &glyph,
                    float italicShear) {
  float padding{1.f};

  float left{glyph.bounds.left - padding};
  float top{glyph.bounds.top - padding};
  float right{glyph.bounds.left + glyph.bounds.width + padding};
  float bottom{glyph.bounds.top + glyph.bounds.height + padding};

  float u1{static_cast<float>(glyph.textureRect.left) - padding};
  float v1{static_cast<float>(glyph.textureRect.top) - padding};
  float u2{static_cast<float>(glyph.textureRect.left + glyph.textureRect.width) + padding};
  float v2{static_cast<float>(glyph.textureRect.top + glyph.textureRect.height) + padding};

  vertices.append(
      sf::Vertex(sf::Vector2f(position.x + left - italicShear * top, position.y + top), color, sf::Vector2f(u1, v1)));
  vertices.append(
      sf::Vertex(sf::Vector2f(position.x + right - italicShear * top, position.y + top), color, sf::Vector2f(u2, v1)));
  vertices.append(sf::Vertex(sf::Vector2f(position.x + left - italicShear * bottom, position.y + bottom), color,
                             sf::Vector2f(u1, v2)));
  vertices.append(sf::Vertex(sf::Vector2f(position.x + left - italicShear * bottom, position.y + bottom), color,
                             sf::Vector2f(u1, v2)));
  vertices.append(
      sf::Vertex(sf::Vector2f(position.x + right - italicShear * top, position.y + top), color, sf::Vector2f(u2, v1)));
  vertices.append(sf::Vertex(sf::Vector2f(position.x + right - italicShear * bottom, position.y + bottom), color,
                             sf::Vector2f(u2, v2)));
}
}

engine::page::page(const system_ptr &rptr, const story_ptr &sptr, check_visibility cv) :
    visible{std::move(cv)},
    story{sptr},
    game_object{rptr},
    audio{rptr},
    printables{},
    active_effects{},
    current_printable{},
    current_character{},
    vertices{sf::Triangles},
    vertices_buffer{sf::Triangles, sf::VertexBuffer::Static},
    debug_bounds_vertices{sf::Lines},
    font_texture_vertices{sf::Triangles},
    bounds{},
    line_shift_animation{
        *this,
        translate_vertical::from(0.f)
            .to(-system->line_spacing)
            .during(60)
            .via(tweeny::easing::sinusoidalInOut)
            .on_step([&] {
              redraw();
              system->delay(system::DEFAULT_DELAY);
            })
    },
    needs_advance{true},
    needs_update{true},
    needs_redraw{true},
    x{system->margin_horizontal},
    y{system->margin_vertical + static_cast<float>(system->font_size)},
    min_x{},
    min_y{},
    max_x{},
    max_y{},
    is_bold{},
    is_underlined{},
    is_strike_through{},
    is_uppercase{},
    displacement{displacement::NONE},
    displacement_mode_end{},
    displacement_mode_end_prev{},
    italic_shear{},
    typing_delay_factor{1.f},
    letter_spacing_factor{1.f},
    text_color{sf::Color::White},
    text_texture{} {
  story->set_store(store());
  story->init();

  if (printables.empty()) {
    throw std::runtime_error("No printables after initialisation");
  }

  current_printable = std::begin(printables);
  rect(current_printable).top = system->margin_vertical;
  rect(current_printable).left = x;

  preprocess(*pointer(current_printable));
}

bool engine::page::can_advance() const {
  return needs_advance;
}

void engine::page::advance() {
  if (line_shift_animation.running()) {
    line_shift_animation.step();
  } else if (end_of_text()) {
    needs_advance = false;
    current_character = 0;

    rect(current_printable).width = max_x - rect(current_printable).left;
    rect(current_printable).height = max_y - rect(current_printable).top - system->line_spacing_margin;

    bounds.height = std::min(bounds.height, bounds.height - system->margin_vertical);

    redraw();
  } else {
    if (current_character != pointer(current_printable)->length() - 1) {
      // Just advance to next character
      current_character++;
    } else {
      // Switch to next printable
      rect(current_printable).width = max_x - rect(current_printable).left;
      rect(current_printable).height = max_y - rect(current_printable).top - system->line_spacing_margin;

      current_printable = std::next(current_printable);
      current_character = 0;

      rect(current_printable).top = y - system->line_spacing_margin;
      rect(current_printable).left = x;

      preprocess(*pointer(current_printable));
    }
    needs_update = true;
  }
}

void engine::page::input() {
  if (const auto &mp{system->mouse_position()}; global_bounds().contains(mp)) {
    apply_mouse_hover(mp);
    if (system->mouse_click_available()) {
      apply_mouse_click(system->mouse_click_position());
    } else {
      redraw();
    }
  }
}

void engine::page::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  ensure_updated();

#ifdef DEBUG
  debug_bounds_vertices.clear();
  draw_page_outline();
  for (auto it{std::begin(printables)}; it != std::end(printables); ++it) {
    draw_printable_outline(it);
  }
#endif

  states.transform *= getTransform();
  states.texture = &system->font->getTexture(system->font_size);

  // Update the vertex buffer if it is being used
  if (sf::VertexBuffer::isAvailable()) {
    if (vertices_buffer.getVertexCount() != vertices.getVertexCount())
      vertices_buffer.create(vertices.getVertexCount());

    if (vertices.getVertexCount() > 0)
      vertices_buffer.update(&vertices[0]);
  }

  if (sf::VertexBuffer::isAvailable()) {
    target.draw(vertices_buffer, states);
  } else {
    target.draw(vertices, states);
  }

#ifdef DEBUG
  target.draw(debug_bounds_vertices);
#endif

  if (needs_update) {
    remove_text_effects(current_character);
  }

  needs_update = false;
}

sf::FloatRect engine::page::local_bounds() const {
  return bounds;
}

sf::FloatRect engine::page::global_bounds() const {
  return getTransform().transformRect(local_bounds());
}

engine::printable_store engine::page::store() {
  return {
      [this](printable_ptr &&ptr) {
        printables.emplace_back(std::move(ptr), sf::FloatRect{});
      },
      [this](printable_ptr &&ptr) {
        auto pid{pointer(current_printable)->get_id()};
        printables.emplace_back(std::move(ptr), sf::FloatRect{});
        current_printable = find_printable(pid);

        preprocess(*gen::last(printables)->first);
      },
      [this](size_t n) {
        if (n >= printables.size()) {
          throw std::runtime_error("Trying to truncate the entire printable store");
        }

        auto pid{pointer(current_printable)->get_id()};
        printables.resize(printables.size() - n);
        current_printable = find_printable(pid);

        if (current_printable == std::end(printables)) {
          current_printable = gen::last(printables);
        }

        y = rect(current_printable).top;
        x = rect(current_printable).left;
        max_y = std::min(max_y, rect(current_printable).top + rect(current_printable).height);
        max_x = std::min(max_x, rect(current_printable).left + rect(current_printable).width);
      },
      [this]() {
        rect(current_printable).width = max_x - rect(current_printable).left;
        rect(current_printable).height = max_y - rect(current_printable).top - system->line_spacing;

        current_printable = std::next(current_printable);
        current_character = 0;

        rect(current_printable).top = y - system->line_spacing_margin;
        rect(current_printable).left = x;

        needs_advance = true;
        needs_update = true;
      },
      [this]() {
        redraw();
      }
  };
}

float engine::page::measure_text(const printable &printable, size_t begin, size_t end) const {
  float width{};
  sf::Uint32 curr_char, prev_char{sf::Utf32::decodeWide(printable[begin])};
  for (size_t i = begin; i < end; ++i) {
    apply_text_effects(printable, i);
    curr_char = sf::Utf32::decodeWide(printable[i]);

    if (curr_char == L'\r')
      continue;

    width += system->font->getKerning(prev_char, curr_char, system->font_size);

    if ((curr_char == L' ') || (curr_char == L'\n') || (curr_char == L'\t')) {
      switch (curr_char) {
        case L' ':
          width += system->whitespace_width + letter_spacing_factor;
          break;
        case L'\t':
          width += system->whitespace_width * 4;
          break;
        case L'\n':
          return width;
          break;
      }
    } else {
      const auto &glyph{system->font->getGlyph(curr_char, system->font_size, is_bold)};
      width += glyph.advance + system->letter_spacing + letter_spacing_factor;
    }

    prev_char = curr_char;
    remove_text_effects(i);
  }

  return width;
}

void engine::page::preprocess(printable &printable) {
  float word_width{};
  auto last_blank{std::numeric_limits<size_t>::max()};
  auto p_len{printable.length()};
  sf::Uint32 curr_char, prev_char{sf::Utf32::decodeWide(printable[0])};

  for (size_t i = 0; i < p_len; ++i) {
    apply_text_effects(printable, i);
    curr_char = sf::Utf32::decodeWide(printable[i]);

    if (curr_char == L'\r')
      continue;

    if (displacement != displacement::NONE && !displacement_mode_end) {
      auto d_effect{displacement_effect(displacement)};
      displacement_mode_end = d_effect->end;
      word_width = 0;
    }

    if (displacement_mode_end && i != displacement_mode_end) {
      continue;
    } else {
      displacement_mode_end = 0;
    }

    word_width += system->font->getKerning(prev_char, curr_char, system->font_size);

    if ((curr_char == L' ') || (curr_char == L'\n') || (curr_char == L'\t')) {
      switch (curr_char) {
        case L' ':
          word_width += system->whitespace_width + letter_spacing_factor;
          if (i) last_blank = i;
          break;
        case L'\t':
          word_width += system->whitespace_width * 4;
          if (i) last_blank = i;
          break;
        case L'\n':
          word_width = 0;
          break;
      }
    } else {
      const auto &glyph{system->font->getGlyph(curr_char, system->font_size, is_bold)};
      word_width += glyph.advance + system->letter_spacing + letter_spacing_factor;
    }

    prev_char = curr_char;
    remove_text_effects(i);

    if (word_width > system->effective_page_width()) {
      if (last_blank == std::numeric_limits<size_t>::max()) {
        printable.inject_line_at(i);
        p_len++;
      } else {
        printable.break_line_at(last_blank);
      }
      word_width = 0;
      last_blank = std::numeric_limits<size_t>::max();
    }
  }
}

void engine::page::ensure_updated() const {
  if (!needs_update)
    return;

  const auto &printable{*pointer(current_printable)};

  apply_text_effects(printable, current_character);

  auto prev_char{sf::Utf32::decodeWide(printable[current_character ? current_character - 1 : 0])};
  auto curr_char{sf::Utf32::decodeWide(printable[current_character])};

  // Skip to avoid glitches
  if (curr_char == L'\r')
    return;

  if (displacement != displacement::NONE && !displacement_mode_end) {
    enum displacement d = displacement;
    auto d_effect{displacement_effect(d)};
    displacement_mode_end = d_effect->end + 1;
    auto width{measure_text(printable, d_effect->begin, d_effect->end)};
    auto spacing{displacement_spacing(d, width)};
    if (displacement_mode_end_prev != current_character - 1) {
      y += system->line_spacing;
    }
    x = system->margin_horizontal + spacing;
    apply_text_effects(printable, current_character);
    displacement_mode_end_prev = displacement_mode_end;
  }

  if (displacement_mode_end && current_character == displacement_mode_end) {
    displacement_mode_end = 0;
    y += system->line_spacing;
    x = system->margin_horizontal;
  }

  x += system->font->getKerning(prev_char, curr_char, system->font_size);

  // Handle special characters
  if ((curr_char == L' ') || (curr_char == L'\n') || (curr_char == L'\t')) {
    min_x = std::min(min_x, x);
    min_y = std::min(min_y, y);

    switch (curr_char) {
      case L' ':
        x += system->whitespace_width + letter_spacing_factor;
        break;
      case L'\t':
        x += system->whitespace_width * 4;
        break;
      case L'\n':
        new_line();
        break;
    }

    max_x = std::max(max_x, x);
    max_y = std::max(max_y, y);
  } else {
    const auto &glyph{system->font->getGlyph(curr_char, system->font_size, is_bold)};

    float left{glyph.bounds.left};
    float top{glyph.bounds.top};
    float right{glyph.bounds.left + glyph.bounds.width};
    float bottom{glyph.bounds.top + glyph.bounds.height};

    min_x = std::min(min_x, x + left - italic_shear * bottom);
    max_x = std::max(max_x, x + right - italic_shear * top);
    min_y = std::min(min_y, y + top);
    max_y = std::max(max_y, y + bottom);

    add_glyph_quad(vertices, sf::Vector2f(x, y), text_color, glyph, italic_shear);
    if (text_texture) {
      add_font_background(font_texture_vertices, sf::Vector2f(x, y), glyph, italic_shear);
    }
    auto advance{glyph.advance + system->letter_spacing + letter_spacing_factor};

    if (is_underlined) {
      add_line(vertices, x, advance, y, text_color, system->underline_offset, system->underline_thickness);
    }

    auto x_bounds{system->font->getGlyph(L'x', system->font_size, is_bold).bounds};
    float strike_through_offset{x_bounds.top + x_bounds.height / 2.f};

    if (is_strike_through) {
      add_line(vertices, x, advance, y, text_color, strike_through_offset, system->underline_thickness);
    }

    x += advance;

    audio.play_typewriter_click();
  }

  bounds.top = min_y;
  bounds.left = min_x;
  bounds.height = max_y - min_y;
  bounds.width = max_x - min_x;

  bounds.height = std::max(bounds.height,
                           bounds.height + system->margin_vertical);
  bounds.width = std::max(bounds.width, bounds.width + system->margin_horizontal);

  bounds.width = std::min(bounds.width,
                          system->page_width);

  system->delay(static_cast<float>(system->typing_delay), typing_delay_factor);
}

void engine::page::apply_text_effects(const printable &printable, size_t idx) const {
  printable.load_effects(idx, std::back_inserter(active_effects));

  for (const auto &e : active_effects) {
    switch (e.kind) {
      case text_effect::kind::BOLD:
        is_bold = true;
        break;
      case text_effect::kind::ITALIC:
        italic_shear = 0.209f;
        break;
      case text_effect::kind::UNDERLINE:
        is_underlined = true;
        break;
      case text_effect::kind::STRIKE_THROUGH:
        is_strike_through = true;
        break;
      case text_effect::kind::UPPERCASE:
        is_uppercase = true;
        break;
      case text_effect::kind::DELAY:
        typing_delay_factor = e.delay_factor;
        break;
      case text_effect::kind::SPACING:
        letter_spacing_factor = e.letter_spacing_factor;
        break;
      case text_effect::kind::COLOR:
        text_color = e.color;
        break;
      case text_effect::kind::TEXTURE:
        text_texture = &system->get_textures(e.texture, "text");
        break;
      case text_effect::kind::CENTER:
        displacement = displacement::CENTER;
        break;
      case text_effect::kind::RIGHT:
        displacement = displacement::RIGHT;
        break;
    }
  }
}

void engine::page::remove_text_effects(size_t idx) const {
  gen::remove_if(active_effects, [&](const auto &e) {
    return idx == e.end;
  });

  is_bold = false;
  is_underlined = false;
  is_strike_through = false;
  is_uppercase = false;
  displacement = displacement::NONE;
  italic_shear = 0.f;
  typing_delay_factor = 1.f;
  letter_spacing_factor = 1.f;
  text_color = sf::Color::White;
  text_texture = nullptr;
}

void engine::page::apply_mouse_hover(const sf::Vector2f &cursor) {
  bool hovering{};
  for (auto &[printable, p_bounds] : printables) {
    auto t{getTransform().transformRect(p_bounds)};
    if (visible(t) && printable->interactive()) {
      if (t.contains(cursor)) {
        printable->on_hover_start();
        hovering = true;
      } else {
        printable->on_hover_end();
      }

      needs_redraw = true;
    }
  }

  system->set_cursor(hovering
                     ? system::cursor::HAND
                     : system::cursor::ARROW);
}

void engine::page::apply_mouse_click(const sf::Vector2f &cursor) {
  for (auto &[printable, p_bounds] : printables) {
    auto t{getTransform().transformRect(p_bounds)};
    if (visible(t) && printable->interactive()) {
      if (t.contains(cursor)) {
        story->act(printable->on_click());
        system->set_cursor(system::cursor::ARROW);
        break;
      }
    }
  }
}

void engine::page::redraw() {
  if (!needs_redraw)
    return;
  needs_redraw = false;

#ifdef DEBUG
  debug_bounds_vertices.clear();
  draw_page_outline();
  for (auto it{std::begin(printables)}; it != std::next(current_printable); ++it) {
    draw_printable_outline(it);
  }
#endif

  vertices.clear();

  x = system->margin_horizontal;
  y = system->margin_vertical + static_cast<float>(system->font_size);

  for (auto it{std::begin(printables)}; it != std::next(current_printable); ++it) {
    const auto &printable{*pointer(it)};

    /*
     * In case we need to redraw in the middle of advancing
     * a printable we need to redraw the last one only up to
     * current_character
     */
    size_t p_end;
    if (it == current_printable) {
      p_end = current_character ? current_character + 1 : printable.length();
    } else {
      p_end = printable.length();
    }
    sf::Uint32 curr_char, prev_char{sf::Utf32::decodeWide(printable[0])};
    for (size_t i = 0; i < p_end; ++i) {
      apply_text_effects(printable, i);
      curr_char = sf::Utf32::decodeWide(printable[i]);

      if (curr_char == L'\r')
        return;

      if (displacement != displacement::NONE && !displacement_mode_end) {
        auto d{displacement};
        auto d_effect{displacement_effect(d)};
        displacement_mode_end = d_effect->end + 1;
        // Call to measure_text invalidates iterators of active_effects,
        // so need to set displacement_mode_end before that
        auto width{measure_text(printable, d_effect->begin, d_effect->end)};
        auto spacing{displacement_spacing(d, width)};
        if (displacement_mode_end_prev != i - 1) {
          y += system->line_spacing;
        }
        x = system->margin_horizontal + spacing;
        apply_text_effects(printable, i);
        displacement_mode_end_prev = displacement_mode_end;
      }

      if (displacement_mode_end && i == displacement_mode_end) {
        displacement_mode_end = 0;
        y += system->line_spacing;
        x = system->margin_horizontal;
      }

      x += system->font->getKerning(prev_char, curr_char, system->font_size);

      if ((curr_char == L' ') || (curr_char == L'\n') || (curr_char == L'\t')) {
        switch (curr_char) {
          case L' ':
            x += system->whitespace_width + letter_spacing_factor;
            break;
          case L'\t':
            x += system->whitespace_width * 4;
            break;
          case L'\n':
            y += system->line_spacing;
            x = system->margin_horizontal;
            break;
        }
      } else {
        const auto &glyph{system->font->getGlyph(curr_char, system->font_size, is_bold)};
        add_glyph_quad(vertices, sf::Vector2f(x, y), text_color, glyph, italic_shear);
        auto advance{glyph.advance + system->letter_spacing + letter_spacing_factor};

        if (is_underlined) {
          add_line(vertices, x, advance, y, text_color, system->underline_offset, system->underline_thickness);
        }

        auto x_bounds{system->font->getGlyph(L'x', system->font_size, is_bold).bounds};
        float strike_through_offset{x_bounds.top + x_bounds.height / 2.f};

        if (is_strike_through) {
          add_line(vertices, x, advance, y, text_color, strike_through_offset, system->underline_thickness);
        }

        x += advance;
      }

      prev_char = curr_char;
      remove_text_effects(i);
    }
  }
}

engine::page::effect_it engine::page::displacement_effect(enum displacement d) const {
  switch (d) {
    case displacement::CENTER:
      return find_effect(text_effect::kind::CENTER);
    case displacement::RIGHT:
      return find_effect(text_effect::kind::RIGHT);
    default:
      throw std::runtime_error("Wrong displacement value");
  }
}

float engine::page::displacement_spacing(enum displacement d, float width) const {
  switch (d) {
    case displacement::CENTER:
      return (system->effective_page_width() - width) / 2;
    case displacement::RIGHT:
      return system->effective_page_width() - width - system->margin_horizontal / 2;
    default:
      throw std::runtime_error("Wrong displacement value");
  }
}

void engine::page::new_line() const {
  y += system->line_spacing;
  x = system->margin_horizontal;
  if (end_of_page()) {
    line_shift_animation.prime();
  }
}

bool engine::page::end_of_page() const {
  return y >= system->page_height - system->margin_vertical;
}

bool engine::page::end_of_text() const {
  return current_character == pointer(current_printable)->length() - 1
         && std::next(current_printable) == std::end(printables);
}

void engine::page::draw_printable_outline(printable_iterator it) const {
  auto trans{getTransform().transformRect(rect(it))};
  debug_bounds_vertices.append(
      sf::Vertex(sf::Vector2f(trans.left, trans.top), sf::Color::Yellow,
                 sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(trans.left, trans.top + trans.height),
      sf::Color::Yellow, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(trans.left, trans.top + trans.height),
      sf::Color::Yellow, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(trans.left + trans.width,
                   trans.top + trans.height),
      sf::Color::Yellow, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(trans.left + trans.width,
                   trans.top + trans.height),
      sf::Color::Yellow, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(trans.left + trans.width, trans.top),
      sf::Color::Yellow, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(trans.left + trans.width, trans.top),
      sf::Color::Yellow, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(
      sf::Vertex(sf::Vector2f(trans.left, trans.top), sf::Color::Yellow,
                 sf::Vector2f(1, 1)));
}

void engine::page::draw_page_outline() const {
  auto root_trans{global_bounds()};
  debug_bounds_vertices.append(
      sf::Vertex(sf::Vector2f(root_trans.left, root_trans.top), sf::Color::Magenta,
                 sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(root_trans.left, root_trans.top + root_trans.height),
      sf::Color::Magenta, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(root_trans.left, root_trans.top + root_trans.height),
      sf::Color::Magenta, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(root_trans.left + root_trans.width,
                   root_trans.top + root_trans.height),
      sf::Color::Magenta, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(root_trans.left + root_trans.width,
                   root_trans.top + root_trans.height),
      sf::Color::Magenta, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(root_trans.left + root_trans.width, root_trans.top),
      sf::Color::Magenta, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(sf::Vertex(
      sf::Vector2f(root_trans.left + root_trans.width, root_trans.top),
      sf::Color::Magenta, sf::Vector2f(1, 1)));
  debug_bounds_vertices.append(
      sf::Vertex(sf::Vector2f(root_trans.left, root_trans.top), sf::Color::Magenta,
                 sf::Vector2f(1, 1)));
}