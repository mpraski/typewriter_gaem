//
// Created by marcin on 7/19/19.
//

#include "page.h"

namespace {
// Add an underline or strikethrough line to the vertex array
void addLine(sf::VertexArray &vertices,
             float lineLength,
             float lineTop,
             const sf::Color &color,
             float offset,
             float thickness,
             float outlineThickness = 0) {
  float top{std::floor(lineTop + offset - (thickness / 2) + 0.5f)};
  float bottom{top + std::floor(thickness + 0.5f)};

  vertices.append(sf::Vertex(sf::Vector2f(-outlineThickness, top - outlineThickness), color, sf::Vector2f(1, 1)));
  vertices.append(
      sf::Vertex(sf::Vector2f(lineLength + outlineThickness, top - outlineThickness), color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(sf::Vector2f(-outlineThickness, bottom + outlineThickness), color, sf::Vector2f(1, 1)));
  vertices.append(sf::Vertex(sf::Vector2f(-outlineThickness, bottom + outlineThickness), color, sf::Vector2f(1, 1)));
  vertices.append(
      sf::Vertex(sf::Vector2f(lineLength + outlineThickness, top - outlineThickness), color, sf::Vector2f(1, 1)));
  vertices.append(
      sf::Vertex(sf::Vector2f(lineLength + outlineThickness, bottom + outlineThickness), color, sf::Vector2f(1, 1)));
}

// Add a glyph quad to the vertex array
void addGlyphQuad(sf::VertexArray &vertices,
                  sf::Vector2f position,
                  const sf::Color &color,
                  const sf::Glyph &glyph,
                  float italicShear,
                  float outlineThickness = 0) {
  float padding{1.f};

  float left{glyph.bounds.left - padding};
  float top{glyph.bounds.top - padding};
  float right{glyph.bounds.left + glyph.bounds.width + padding};
  float bottom{glyph.bounds.top + glyph.bounds.height + padding};

  float u1{static_cast<float>(glyph.textureRect.left) - padding};
  float v1{static_cast<float>(glyph.textureRect.top) - padding};
  float u2{static_cast<float>(glyph.textureRect.left + glyph.textureRect.width) + padding};
  float v2{static_cast<float>(glyph.textureRect.top + glyph.textureRect.height) + padding};

  vertices.append(sf::Vertex(
      sf::Vector2f(position.x + left - italicShear * top - outlineThickness, position.y + top - outlineThickness),
      color, sf::Vector2f(u1, v1)));
  vertices.append(sf::Vertex(
      sf::Vector2f(position.x + right - italicShear * top - outlineThickness, position.y + top - outlineThickness),
      color, sf::Vector2f(u2, v1)));
  vertices.append(sf::Vertex(
      sf::Vector2f(position.x + left - italicShear * bottom - outlineThickness, position.y + bottom - outlineThickness),
      color, sf::Vector2f(u1, v2)));
  vertices.append(sf::Vertex(
      sf::Vector2f(position.x + left - italicShear * bottom - outlineThickness, position.y + bottom - outlineThickness),
      color, sf::Vector2f(u1, v2)));
  vertices.append(sf::Vertex(
      sf::Vector2f(position.x + right - italicShear * top - outlineThickness, position.y + top - outlineThickness),
      color, sf::Vector2f(u2, v1)));
  vertices.append(sf::Vertex(sf::Vector2f(position.x + right - italicShear * bottom - outlineThickness,
                                          position.y + bottom - outlineThickness), color, sf::Vector2f(u2, v2)));
}
}

engine::page::page(const resources_ptr &rptr, std::vector<printable *> &&ps) :
    game_state{rptr},
    audio{rptr},
    buffer{},
    printables{},
    active_effects{},
    new_effects(),
    global_effects{},
    current_printable{},
    current_character{},
    checked_character{},
    vertices{sf::Triangles},
    vertices_buffer{sf::Triangles, sf::VertexBuffer::Static},
    bounds{},
    end_of_text{},
    needs_update{true},
    x{resources->margin_horizontal},
    y{resources->margin_vertical + static_cast<float>(resources->font_size)},
    min_x{resources->margin_horizontal + static_cast<float>(resources->font_size)},
    min_y{resources->margin_vertical + static_cast<float>(resources->font_size)},
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
    typing_delay_factor{1.f},
    letter_spacing_factor{1.f},
    text_color{sf::Color::White} {
  whitespace_width += letter_spacing;

  for (auto p : ps) printables.emplace_back(p, sf::FloatRect{}, 0u);

  if (printables.empty()) {
    throw std::runtime_error("Printables cannot be empty");
  }

  current_printable = std::begin(printables);
  rect(current_printable).top = resources->margin_vertical;
  rect(current_printable).left = x;
}

bool engine::page::text_end() const {
  return end_of_text;
}

void engine::page::advance() {
  if (current_character == pointer(current_printable)->length() - 1
      && std::next(current_printable) == std::end(printables)) {
    end_of_text = true;

    rect(current_printable).width = max_x - rect(current_printable).left;
    rect(current_printable).height = max_y - rect(current_printable).top + static_cast<float>(resources->font_size) / 2;
  } else {
    if (current_character != pointer(current_printable)->length() - 1) {
      current_character++;
    } else {
      rect(current_printable).width = max_x - rect(current_printable).left;
      rect(current_printable).height = max_y - rect(current_printable).top;;

      if (current_printable == std::begin(printables)) {
        rect(current_printable).height += line_spacing;
      }

      current_printable = std::next(current_printable);
      checked_character = current_character = 0;

      index(current_printable) = buffer.length();
    }
    needs_update = true;
  }
}

void engine::page::input() {
  apply_mouse_hover(resources->mouse_position());
  redraw();
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

  remove_text_effects();
}

// Measure the length of the next word, if it overflows move it the next line
void engine::page::ensure_line_break(const printable &printable) const {
  if (current_character <= checked_character)
    return;

  size_t curr_char{current_character};
  size_t prev_char{current_character};
  float word_width{};

  do {
    if (printable[curr_char] == L'\r')
      continue;

    printf("(%c)", printable[curr_char]);

    word_width += resources->font->getKerning(printable[prev_char], printable[curr_char], resources->font_size);

    switch (printable[curr_char]) {
      case L' ':
        word_width += whitespace_width + letter_spacing_factor;
        break;
      case L'\t':
        word_width += whitespace_width * 4;
        break;
      default:
        const auto &glyph{resources->font->getGlyph(printable[curr_char], resources->font_size, is_bold)};
        word_width += glyph.advance + letter_spacing + letter_spacing_factor;
        break;
    }

    checked_character = prev_char = curr_char;
  } while (!std::iswspace(printable[curr_char++]));

  printf("\nx: %f, y: %f, word_width: %f\n", x, y, word_width);

  if (x + word_width + resources->margin_horizontal >= resources->page_width) {
    new_line();

    min_x = std::min(min_x, x);
    min_y = std::min(min_y, y);
    max_x = std::max(max_x, x);
    max_y = std::max(max_y, y);
  }
}

void engine::page::ensure_updated() const {
  if (!needs_update)
    return;

  needs_update = false;

  wchar_t prev_char{(*pointer(current_printable))[current_character ? current_character - 1u : 0u]};
  wchar_t curr_char{(*pointer(current_printable))[current_character]};

  // Skip to avoid glitches
  if (curr_char == L'\r')
    return;

  // If this is the new printable, add a newline
  if (current_printable != std::begin(printables) && !current_character) {
    new_line();
  } else {
    x += resources->font->getKerning(prev_char, curr_char, resources->font_size);
  }

  // Ensure line is broken if next word exceeds page width
  ensure_line_break(*pointer(current_printable));
  // Load text effects starting at this position
  apply_text_effects(*pointer(current_printable));
  // Add to buffer
  buffer.push(curr_char);

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
    const auto &glyph = resources->font->getGlyph(curr_char, resources->font_size, is_bold);
    addGlyphQuad(vertices, sf::Vector2f(x, y), text_color, glyph, italic_shear);
    x += glyph.advance + letter_spacing + letter_spacing_factor;

    audio.play_typewriter_click();
  }

  delay();
}

void engine::page::load_global_text_effects(printable_iterator it) {
  const auto &printable{*pointer(it)};
  auto idx{index(it)};

  for (size_t i{0}; i < printable.length(); ++i) {
    new_effects.clear();
    printable.load_effects(i, std::back_inserter(new_effects));

    if (!new_effects.empty()) {
      global_effects[idx].clear();
      for (const auto &e : new_effects) {
        global_effects[idx].push_back(e.to_page_coords(idx));
      }
    }

    idx++;
  }
}

void engine::page::apply_global_text_effects(size_t idx) const {
  active_effects.insert(
      std::end(active_effects),
      std::begin(global_effects[idx]),
      std::end(global_effects[idx])
  );

  set_text_variables();
}

void engine::page::apply_text_effects(const printable &printable) const {
  new_effects.clear();
  printable.load_effects(current_character, std::back_inserter(new_effects));

  for (const auto &e : new_effects) {
    auto pos{buffer.length() ? buffer.length() - 1 : 0u};
    global_effects[pos].push_back(e.to_page_coords(pos));
  }

  active_effects.insert(
      std::end(active_effects),
      std::begin(new_effects),
      std::end(new_effects)
  );

  set_text_variables();
}

void engine::page::set_text_variables() const {
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
    }
  }
}

void engine::page::remove_global_text_effects(size_t idx) const {
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

  unset_text_variables();
}

void engine::page::remove_text_effects() const {
  active_effects.erase(
      std::remove_if(
          std::begin(active_effects),
          std::end(active_effects),
          [&](const auto &e) {
            return current_character == e.end;
          }
      ),
      std::end(active_effects)
  );

  unset_text_variables();
}

void engine::page::unset_text_variables() const {
  is_bold = false;
  is_underlined = false;
  is_strike_through = false;
  is_uppercase = false;
  italic_shear = 0.f;
  typing_delay_factor = 1.f;
  letter_spacing_factor = 1.f;
  text_color = sf::Color::White;
}

void engine::page::delay() const {
  sf::Clock clock;
  sf::Time time;

  while (time.asMilliseconds() < resources->typing_delay * typing_delay_factor) {
    time += clock.getElapsedTime();
  }
}

void engine::page::apply_mouse_hover(sf::Vector2i cursor) {
  for (auto p{std::begin(printables)}; p != std::end(printables); ++p) {
    auto &printable{*pointer(p)};
    auto &pbounds{rect(p)};

    // To-Do: Only check printables in current viewport
    if (printable.interactive()) {
      if (pbounds.contains(cursor.x, cursor.y)) {
        printable.on_hover_start();
      } else {
        printable.on_hover_end();
      }

      if (printable.needs_update()) {
        load_global_text_effects(p);
      }
    }
  }
}

void engine::page::redraw() {
  vertices.clear();

  x = resources->margin_horizontal;
  y = resources->margin_vertical + static_cast<float>(resources->font_size);

  wchar_t curr_char, prev_char{buffer[0]};

  auto range{buffer.from(0u)};
  for (size_t i{0}; i < range.length(); ++i) {
    apply_global_text_effects(i);
    curr_char = range[i];

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
      const auto &glyph = resources->font->getGlyph(curr_char, resources->font_size, is_bold);
      addGlyphQuad(vertices, sf::Vector2f(x, y), text_color, glyph, italic_shear);
      x += glyph.advance + letter_spacing + letter_spacing_factor;
    }

    prev_char = curr_char;
    remove_global_text_effects(i);
  }
}