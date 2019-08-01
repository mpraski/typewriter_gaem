//
// Created by marcin on 7/19/19.
//

#include "page.h"

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

engine::page::page(const resources_ptr &rptr, std::vector<printable *> &&ps) :
    game_state{rptr},
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
    end_of_text{},
    needs_update{true},
    needs_redraw{true},
    x{resources->margin_horizontal},
    y{resources->margin_vertical + static_cast<float>(resources->font_size)},
    min_x{},
    min_y{},
    max_x{},
    max_y{},
    is_bold{},
    is_underlined{},
    is_strike_through{},
    is_uppercase{},
    is_center{},
    italic_shear{},
    whitespace_width{resources->font->getGlyph(L' ', resources->font_size, false).advance},
    letter_spacing{(whitespace_width / 3.f) * (resources->letter_spacing_factor - 1.f)},
    line_spacing{resources->font->getLineSpacing(resources->font_size) * resources->line_spacing_factor},
    line_spacing_margin{line_spacing / 1.3f},
    underline_offset{resources->font->getUnderlinePosition(resources->font_size)},
    underline_thickness{resources->font->getUnderlineThickness(resources->font_size)},
    typing_delay_factor{1.f},
    letter_spacing_factor{1.f},
    text_color{sf::Color::White},
    text_texture{} {
  whitespace_width += letter_spacing;

  for (auto p : ps) printables.emplace_back(p, sf::FloatRect{});

  if (printables.empty()) {
    throw std::runtime_error("Printables cannot be empty");
  }

  current_printable = std::begin(printables);
  rect(current_printable).top = resources->margin_vertical;
  rect(current_printable).left = x;

  preprocess(*pointer(current_printable));
}

bool engine::page::text_end() const {
  return end_of_text;
}

void engine::page::advance() {
  if (current_character == pointer(current_printable)->length() - 1
      && std::next(current_printable) == std::end(printables)) {
    end_of_text = true;

    rect(current_printable).width = max_x - rect(current_printable).left;
    rect(current_printable).height = max_y - rect(current_printable).top - line_spacing_margin;

#ifdef DEBUG
    auto trans{getTransform().transformRect(rect(current_printable))};
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
#endif

    redraw();
  } else {
    if (current_character != pointer(current_printable)->length() - 1) {
      current_character++;
    } else {
      rect(current_printable).width = max_x - rect(current_printable).left;
      rect(current_printable).height = max_y - rect(current_printable).top - line_spacing_margin;

#ifdef DEBUG
      auto trans{getTransform().transformRect(rect(current_printable))};
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
#endif

      current_printable = std::next(current_printable);
      current_character = 0;

      rect(current_printable).top = y - line_spacing / 1.3f;
      rect(current_printable).left = x;

      preprocess(*pointer(current_printable));
    }
    needs_update = true;
  }
}

void engine::page::input() {
  if (auto mp{resources->mouse_position()}; global_bounds().contains(mp)) {
    apply_mouse_position(mp);
    redraw();
  }
}

void engine::page::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  ensure_updated();

  states.transform *= getTransform();
  states.texture = &resources->font->getTexture(resources->font_size);

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
  auto bounds_with_margin{bounds};
  bounds_with_margin.height += resources->margin_vertical;
  bounds_with_margin.width += resources->margin_horizontal;
  return bounds_with_margin;
}

sf::FloatRect engine::page::global_bounds() const {
  return getTransform().transformRect(local_bounds());
}

void engine::page::preprocess(printable &printable) const {
  auto last_blank{std::numeric_limits<size_t>::max()};
  auto word_width{resources->margin_horizontal * 2};
  auto p_len{printable.length()};
  sf::Uint32 curr_char, prev_char{sf::Utf32::decodeWide(printable[0])};

  for (size_t i{0}; i < p_len; ++i) {
    apply_text_effects(printable, i);
    curr_char = sf::Utf32::decodeWide(printable[i]);

    if (curr_char == L'\r')
      continue;

    word_width += resources->font->getKerning(prev_char, curr_char, resources->font_size);

    if ((curr_char == L' ') || (curr_char == L'\n') || (curr_char == L'\t')) {
      switch (curr_char) {
        case L' ':
          word_width += whitespace_width + letter_spacing_factor;
          if (i) last_blank = i;
          break;
        case L'\t':
          word_width += whitespace_width * 4;
          if (i) last_blank = i;
          break;
        case L'\n':
          word_width = resources->margin_horizontal * 2;
          break;
      }
    } else {
      const auto &glyph{resources->font->getGlyph(curr_char, resources->font_size, is_bold)};
      word_width += glyph.advance + letter_spacing + letter_spacing_factor;
    }

    prev_char = curr_char;
    remove_text_effects(i);

    if (word_width >= resources->page_width) {
      if (last_blank == std::numeric_limits<size_t>::max()) {
        printable.inject_line_at(i);
        p_len++;
      } else {
        printable.break_line_at(last_blank);
      }
      word_width = resources->margin_horizontal * 2;
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

  x += resources->font->getKerning(prev_char, curr_char, resources->font_size);

  // Handle special characters
  if ((curr_char == L' ') || (curr_char == L'\n') || (curr_char == L'\t')) {
    min_x = std::min(min_x, x);
    min_y = std::min(min_y, y);

    switch (curr_char) {
      case L' ':
        x += whitespace_width + letter_spacing_factor;
        break;
      case L'\t':
        x += whitespace_width * 4;
        break;
      case L'\n':
        y += line_spacing;
        x = resources->margin_horizontal;
        break;
    }

    max_x = std::max(max_x, x);
    max_y = std::max(max_y, y);
  } else {
    const auto &glyph{resources->font->getGlyph(curr_char, resources->font_size, is_bold)};

    float left{glyph.bounds.left};
    float top{glyph.bounds.top};
    float right{glyph.bounds.left + glyph.bounds.width};
    float bottom{glyph.bounds.top + glyph.bounds.height};

    min_x = std::min(min_x, x + left - italic_shear * bottom);
    max_x = std::max(max_x, x + right - italic_shear * top);
    min_y = std::min(min_y, y + top);
    max_y = std::max(max_y, y + bottom);

    bounds.top = min_y;
    bounds.left = min_x;
    bounds.height = max_y - min_y;
    bounds.width = max_x - min_x;

    add_glyph_quad(vertices, sf::Vector2f(x, y), text_color, glyph, italic_shear);
    if (text_texture) {
      add_font_background(font_texture_vertices, sf::Vector2f(x, y), glyph, italic_shear);
    }
    auto advance{glyph.advance + letter_spacing + letter_spacing_factor};

    if (is_underlined) {
      add_line(vertices, x, advance, y, text_color, underline_offset, underline_thickness);
    }

    auto x_bounds{resources->font->getGlyph(L'x', resources->font_size, is_bold).bounds};
    float strike_through_offset{x_bounds.top + x_bounds.height / 2.f};

    if (is_strike_through) {
      add_line(vertices, x, advance, y, text_color, strike_through_offset, underline_thickness);
    }

    x += advance;

    audio.play_typewriter_click();
  }

  delay();
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
      case text_effect::kind::CENTER:
        break;
      case text_effect::kind::TEXTURE:
        text_texture = &resources->get_textures("text").at(e.texture);
        break;
    }
  }
}

void engine::page::remove_text_effects(size_t idx) const {
  active_effects.erase(
      std::remove_if(
          std::begin(active_effects),
          std::end(active_effects),
          [&](const auto &e) {
            return idx == e.end;
          }
      ),
      std::end(active_effects)
  );

  is_bold = false;
  is_underlined = false;
  is_strike_through = false;
  is_uppercase = false;
  italic_shear = 0.f;
  typing_delay_factor = 1.f;
  letter_spacing_factor = 1.f;
  text_color = sf::Color::White;
  text_texture = nullptr;
}

void engine::page::delay() const {
  sf::Clock clock;
  sf::Time time;

  while (time.asMilliseconds() < static_cast<float>(resources->typing_delay) * typing_delay_factor) {
    time += clock.getElapsedTime();
  }
}

void engine::page::add_printable(printable_ptr &&ptr) {
  printables.emplace_back(std::move(ptr), sf::FloatRect{});

  current_printable = std::prev(std::end(printables));
  current_character = 0;

  preprocess(*pointer(current_printable));

  needs_update = true;
  end_of_text = false;
}

void engine::page::truncate_printables(size_t n) {
  if (n <= printables.size()) {
    printables.resize(printables.size() - n);

    current_printable = std::prev(std::end(printables));
    current_character = 0;

    needs_update = true;
    end_of_text = false;
  }
}

void engine::page::apply_mouse_position(sf::Vector2f cursor) {
  for (auto p{std::begin(printables)}; p != std::end(printables); ++p) {
    auto &printable{*pointer(p)};
    auto &p_bounds{rect(p)};

    // To-Do: Only check printables in current viewport
    if (printable.interactive()) {
      if (getTransform().transformRect(p_bounds).contains(cursor)) {
        printable.on_hover_start();
      } else {
        printable.on_hover_end();
      }

      needs_redraw = true;
    }
  }
}

void engine::page::redraw() {
  if (!needs_redraw)
    return;
  needs_redraw = false;

  vertices.clear();

  x = resources->margin_horizontal;
  y = resources->margin_vertical + static_cast<float>(resources->font_size);

  for (auto it{std::begin(printables)}; it != std::end(printables); ++it) {
    const auto &printable{*pointer(it)};

    sf::Uint32 curr_char, prev_char{sf::Utf32::decodeWide(printable[0])};
    for (size_t i{0}; i < printable.length(); ++i) {
      apply_text_effects(printable, i);
      curr_char = sf::Utf32::decodeWide(printable[i]);

      if (curr_char == L'\r')
        return;

      x += resources->font->getKerning(prev_char, curr_char, resources->font_size);

      if ((curr_char == L' ') || (curr_char == L'\n') || (curr_char == L'\t')) {
        switch (curr_char) {
          case L' ':
            x += whitespace_width + letter_spacing_factor;
            break;
          case L'\t':
            x += whitespace_width * 4;
            break;
          case L'\n':
            y += line_spacing;
            x = resources->margin_horizontal;
            break;
        }
      } else {
        const auto &glyph{resources->font->getGlyph(curr_char, resources->font_size, is_bold)};
        add_glyph_quad(vertices, sf::Vector2f(x, y), text_color, glyph, italic_shear);
        auto advance{glyph.advance + letter_spacing + letter_spacing_factor};

        if (is_underlined) {
          add_line(vertices, x, advance, y, text_color, underline_offset, underline_thickness);
        }

        auto x_bounds{resources->font->getGlyph(L'x', resources->font_size, is_bold).bounds};
        float strike_through_offset{x_bounds.top + x_bounds.height / 2.f};

        if (is_strike_through) {
          add_line(vertices, x, advance, y, text_color, strike_through_offset, underline_thickness);
        }

        x += advance;
      }

      prev_char = curr_char;
      remove_text_effects(i);
    }
  }
}