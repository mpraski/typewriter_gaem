//
// Created by marcin on 7/24/19.
//

#include "printable.hpp"

#include <utility>
#include "../utilities/General.hpp"

engine::printable::builder::builder()
    : _system{},
      _contents{},
      _font_size{},
      _effects{},
      _interactive{} {

}

engine::printable::builder &engine::printable::builder::with_system(const SystemPtr &sys_ptr) {
  _system = sys_ptr;
  return *this;
}

engine::printable::builder &engine::printable::builder::with_audio(const audio_system_ptr &audio_ptr) {
  _audio = audio_ptr;
  return *this;
}

engine::printable::builder &engine::printable::builder::with_contents(std::wstring c) {
  _contents = std::move(c);
  return *this;
}

engine::printable::builder &engine::printable::builder::with_font_size(unsigned fz) {
  _font_size = fz;
  return *this;
}

engine::printable::builder &engine::printable::builder::with_effects(std::vector<engine::TextEffect> &&ef) {
  _effects = std::move(ef);
  return *this;
}

engine::printable::builder &engine::printable::builder::interactive(bool in) {
  _interactive = in;
  return *this;
}

engine::printable::printable(builder &b)
    : GameObject{b._system},
      audio{b._audio},
      current_character{},
      active_effects{},
      vertices{sf::Triangles},
      vertices_buffer{sf::Triangles, sf::VertexBuffer::Static},
      font_size{mSystem->font_size},
      line_spacing{mSystem->line_spacing},
      line_spacing_margin{mSystem->line_spacing_margin},
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
      needs_update{true},
      needs_redraw{true},
      is_current{},
      x{mSystem->margin_horizontal},
      y{mSystem->margin_vertical + static_cast<float>(mSystem->font_size)},
      min_x{},
      min_y{},
      max_x{},
      max_y{},
      contents{std::move(b._contents)},
      effects{},
      is_interactive{b._interactive} {
  for (const auto &e: b._effects) {
    effects[e.mBegin].push_back(e);
  }

  trim_start(contents);
  trim_end(contents);
  add_end_new_line(contents);

  preprocess();

  listen<GOId>([&](const auto &id) {
    is_current = *this == id;
  });
}

const std::wstring &engine::printable::get_contents() const {
  return contents;
}

size_t engine::printable::length() const {
  return contents.length();
}

size_t engine::printable::effective_length() const {
  return contents.back() == L'\n' ? contents.length() - 1 : contents.length();
}

std::optional<unsigned> engine::printable::get_font_size() const {
  return font_size;
}

bool engine::printable::interactive() const {
  return is_interactive;
}

void engine::printable::load_effects(size_t idx) {
  if (auto k{effects.find(idx)}; k != effects.end())
    std::copy(
        std::cbegin(k->second),
        std::cend(k->second),
        std::back_inserter(active_effects)
    );
}

void engine::printable::break_line_at(size_t idx) {
  if (!std::iswblank(contents[idx])) {
    throw std::runtime_error(gen::str("Symbol ", contents[idx], " is not blank"));
  }
  contents[idx] = L'\n';
}

void engine::printable::inject_line_at(size_t idx) {
  contents.insert(contents.begin() + idx, L'\n');
  offset_effects(idx);
}

void engine::printable::on_hover_start() {

}

void engine::printable::on_hover_end() {

}

engine::action engine::printable::on_click() {
  return {};
}

void engine::printable::offset_effects(size_t idx, int amount) {
  effect_map new_effects;
  new_effects.reserve(effects.size());
  for (auto &[k, v] : effects) {
    auto modified_key{k};
    auto modified_effects{std::move(v)};
    if (k >= idx) {
      modified_key += amount;
      for (auto &e : modified_effects) {
        e.begin += amount;
        e.end += amount;
      }
    }
    new_effects.insert(std::make_pair(modified_key, std::move(modified_effects)));
  }
  effects = new_effects;
}

engine::printable &engine::printable::with_font_size(unsigned fz) {
  font_size = fz;
}


float engine::printable::measure_text(size_t begin, size_t end) {
  float width{};
  sf::Uint32 curr_char, prev_char{sf::Utf32::decodeWide(contents[begin])};
  for (size_t i = begin; i < end; ++i) {
    apply_text_effects(i);
    curr_char = sf::Utf32::decodeWide(contents[i]);

    if (curr_char == L'\r')
      continue;

    width += mSystem->font->getKerning(prev_char, curr_char, font_size);

    if ((curr_char == L' ') || (curr_char == L'\n') || (curr_char == L'\t')) {
      switch (curr_char) {
        case L' ':
          width += mSystem->whitespace_width + letter_spacing_factor;
          break;
        case L'\t':
          width += mSystem->whitespace_width * 4.f;
          break;
        case L'\n':
          return width;
          break;
      }
    } else {
      const auto &glyph{mSystem->font->getGlyph(curr_char, font_size, is_bold)};
      width += glyph.advance + mSystem->letter_spacing + letter_spacing_factor;
    }

    prev_char = curr_char;
    remove_text_effects(i);
  }

  return width;
}

void engine::printable::preprocess() {
  float word_width{};
  float from_last_blank{};
  auto last_blank{std::numeric_limits<size_t>::max()};
  auto p_len{contents.length()};
  sf::Uint32 curr_char, prev_char{sf::Utf32::decodeWide(contents[0])};

  for (size_t i = 0; i < p_len; ++i) {
    apply_text_effects(i);
    curr_char = sf::Utf32::decodeWide(contents[i]);

    if (curr_char == L'\r')
      continue;

    if (displacement != displacement::NONE && !displacement_mode_end) {
      auto d_effect{displacement_effect(displacement)};
      displacement_mode_end = d_effect->mEnd;
      word_width = 0;
    }

    if (displacement_mode_end && i != displacement_mode_end) {
      continue;
    } else {
      displacement_mode_end = 0;
    }

    word_width += mSystem->font->getKerning(prev_char, curr_char, font_size);

    if ((curr_char == L' ') || (curr_char == L'\n') || (curr_char == L'\t')) {
      switch (curr_char) {
        case L' ':
          word_width += mSystem->whitespace_width + letter_spacing_factor;
          if (i) {
            last_blank = i;
            from_last_blank = word_width;
          }
          break;
        case L'\t':
          word_width += mSystem->whitespace_width * 4.f;
          if (i) {
            last_blank = i;
            from_last_blank = word_width;
          }
          break;
        case L'\n':
          word_width = 0;
          break;
      }
    } else {
      const auto &glyph{mSystem->font->getGlyph(curr_char, font_size, is_bold)};
      word_width += glyph.advance + mSystem->letter_spacing + letter_spacing_factor;
    }

    prev_char = curr_char;
    remove_text_effects(i);

    if (word_width > parentLocalBounds().width - 2.f * mSystem->margin_horizontal) {
      if (last_blank == std::numeric_limits<size_t>::max()) {
        inject_line_at(i);
        p_len++;
        word_width = 0;
      } else {
        break_line_at(last_blank);
        word_width -= from_last_blank;
        from_last_blank = 0;
      }
      last_blank = std::numeric_limits<size_t>::max();
    }
  }
}

void engine::printable::ensure_updated() {
  if (!needs_update)
    return;

  apply_text_effects(current_character);

  auto prev_char{sf::Utf32::decodeWide(contents[current_character ? current_character - 1 : 0])};
  auto curr_char{sf::Utf32::decodeWide(contents[current_character])};

  // Skip to avoid glitches
  if (curr_char == L'\r')
    return;

  if (displacement != displacement::NONE && !displacement_mode_end) {
    enum displacement d = displacement;
    auto d_effect{displacement_effect(d)};
    displacement_mode_end = d_effect->mEnd + 1;
    auto width{measure_text(d_effect->mBegin, d_effect->mEnd)};
    auto spacing{displacement_spacing(d, width)};
    if (displacement_mode_end_prev != current_character - 1) {
      y += mSystem->line_spacing;
    }
    x = mSystem->margin_horizontal + spacing;
    apply_text_effects(current_character);
    displacement_mode_end_prev = displacement_mode_end;
  }

  if (displacement_mode_end && current_character == displacement_mode_end) {
    displacement_mode_end = 0;
    y += mSystem->line_spacing;
    x = mSystem->margin_horizontal;
  }

  x += mSystem->font->getKerning(prev_char, curr_char, font_size);

  // Handle special characters
  if ((curr_char == L' ') || (curr_char == L'\n') || (curr_char == L'\t')) {
    min_x = std::min(min_x, x);
    min_y = std::min(min_y, y);

    switch (curr_char) {
      case L' ':
        x += mSystem->whitespace_width + letter_spacing_factor;
        break;
      case L'\t':
        x += mSystem->whitespace_width * 4.f;
        break;
      case L'\n':
        new_line();
        break;
    }

    max_x = std::max(max_x, x);
    max_y = std::max(max_y, y);
  } else {
    const auto &glyph{mSystem->font->getGlyph(curr_char, font_size, is_bold)};

    float left{glyph.bounds.left};
    float top{glyph.bounds.top};
    float right{glyph.bounds.left + glyph.bounds.width};
    float bottom{glyph.bounds.top + glyph.bounds.height};

    min_x = std::min(min_x, x + left - italic_shear * bottom);
    max_x = std::max(max_x, x + right - italic_shear * top);
    min_y = std::min(min_y, y + top);
    max_y = std::max(max_y, y + bottom);

    gen::addGlyphQuad(vertices, sf::Vector2f(x, y), text_color, glyph, italic_shear);
    //if (text_texture) {
    //  gen::add_font_background(font_texture_vertices, sf::Vector2f(x, y), glyph, italic_shear);
    //}
    auto advance{glyph.advance + mSystem->letter_spacing + letter_spacing_factor};

    if (is_underlined) {
      gen::addLine(vertices, x, advance, y, text_color, mSystem->underline_offset, mSystem->underline_thickness);
    }

    auto x_bounds{mSystem->font->getGlyph(L'x', font_size, is_bold).bounds};
    float strike_through_offset{x_bounds.top + x_bounds.height / 2.f};

    if (is_strike_through) {
      gen::addLine(vertices, x, advance, y, text_color, strike_through_offset, mSystem->underline_thickness);
    }

    x += advance;

    audio->play_typewriter_click();
  }

  mBounds.top = min_y;
  mBounds.left = min_x;
  mBounds.height = max_y - min_y;
  mBounds.width = max_x - min_x;

  mBounds.height = std::max(mBounds.height, mBounds.height + mSystem->margin_vertical);
  mBounds.width = std::max(mBounds.width, mBounds.width + mSystem->margin_horizontal);

  mBounds.width = std::min(mBounds.width, parentLocalBounds().width);
}

void engine::printable::apply_text_effects(size_t idx) {
  load_effects(idx);

  typing_delay_factor = static_cast<float>(mSystem->typing_delay);
  font_size = mSystem->font_size;

  for (const auto &e : active_effects) {
    switch (e.Kind) {
      case TextEffect::Kind::BOLD:
        is_bold = true;
        break;
      case TextEffect::Kind::ITALIC:
        italic_shear = 0.209f;
        break;
      case TextEffect::Kind::UNDERLINE:
        is_underlined = true;
        break;
      case TextEffect::Kind::STRIKE_THROUGH:
        is_strike_through = true;
        break;
      case TextEffect::Kind::UPPERCASE:
        is_uppercase = true;
        break;
      case TextEffect::Kind::DELAY:
        typing_delay_factor = static_cast<float>(mSystem->typing_delay) * e.mDelayFactor;
        break;
      case TextEffect::Kind::SPACING:
        letter_spacing_factor = e.mLetterSpacingFactor;
        break;
      case TextEffect::Kind::COLOR:
        text_color = e.mColor;
        break;
      case TextEffect::Kind::TEXTURE:
        //text_texture = &mSystem->get_textures(e.mTexture, "text");
        break;
      case TextEffect::Kind::CENTER:
        displacement = displacement::CENTER;
        break;
      case TextEffect::Kind::RIGHT:
        displacement = displacement::RIGHT;
        break;
      case TextEffect::Kind::FONT_SIZE:
        //font_size = std::lround(static_cast<float>(mSystem->font_size) * e.mFontSizeFactor);
        //fsize(current_printable) = font_size;
        break;
    }
  }
}

void engine::printable::remove_text_effects(size_t idx) const {
  gen::remove_if(active_effects, [&](const auto &e) {
    return idx == e.end;
  });

  is_bold = false;
  is_underlined = false;
  is_strike_through = false;
  is_uppercase = false;
  displacement = displacement::NONE;
  italic_shear = 0.f;
  letter_spacing_factor = 1.f;
  text_color = sf::Color::White;
  //text_texture = nullptr;
}

void engine::printable::redraw() {
  if (!needs_redraw)
    return;
  needs_redraw = false;

#ifdef DEBUG
  debug_bounds_vertices.clear();
#endif

  x = mSystem->margin_horizontal;
  y = mSystem->margin_vertical + static_cast<float>(font_size);

  /*
   * In case we need to redraw in the middle of advancing
   * a printable we need to redraw the last one only up to
   * current_character
   */

  sf::Uint32 curr_char, prev_char{sf::Utf32::decodeWide(contents[0])};
  for (size_t i = 0; i < contents.length(); ++i) {
    apply_text_effects(i);
    curr_char = sf::Utf32::decodeWide(contents[i]);

    if (curr_char == L'\r')
      return;

    if (displacement != displacement::NONE && !displacement_mode_end) {
      auto d{displacement};
      auto d_effect{displacement_effect(d)};
      displacement_mode_end = d_effect->mEnd + 1;
      // Call to measure_text invalidates iterators of active_effects,
      // so need to set displacement_mode_end before that
      auto width{measure_text(d_effect->mBegin, d_effect->mEnd)};
      auto spacing{displacement_spacing(d, width)};
      if (displacement_mode_end_prev != i - 1) {
        y += mSystem->line_spacing;
      }
      x = mSystem->margin_horizontal + spacing;
      apply_text_effects(i);
      displacement_mode_end_prev = displacement_mode_end;
    }

    if (displacement_mode_end && i == displacement_mode_end) {
      displacement_mode_end = 0;
      y += mSystem->line_spacing;
      x = mSystem->margin_horizontal;
    }

    x += mSystem->font->getKerning(prev_char, curr_char, font_size);

    if ((curr_char == L' ') || (curr_char == L'\n') || (curr_char == L'\t')) {
      switch (curr_char) {
        case L' ':
          x += mSystem->whitespace_width + letter_spacing_factor;
          break;
        case L'\t':
          x += mSystem->whitespace_width * 4.f;
          break;
        case L'\n':
          y += mSystem->line_spacing;
          x = mSystem->margin_horizontal;
          break;
      }
    } else {
      const auto &glyph{mSystem->font->getGlyph(curr_char, font_size, is_bold)};
      gen::addGlyphQuad(vertices, sf::Vector2f(x, y), text_color, glyph, italic_shear);
      auto advance{glyph.advance + mSystem->letter_spacing + letter_spacing_factor};

      if (is_underlined) {
        gen::addLine(vertices, x, advance, y, text_color, mSystem->underline_offset, mSystem->underline_thickness);
      }

      auto x_bounds{mSystem->font->getGlyph(L'x', font_size, is_bold).bounds};
      float strike_through_offset{x_bounds.top + x_bounds.height / 2.f};

      if (is_strike_through) {
        gen::addLine(vertices, x, advance, y, text_color, strike_through_offset, mSystem->underline_thickness);
      }

      x += advance;
    }

    prev_char = curr_char;
    remove_text_effects(i);
  }
}

engine::printable::effect_it engine::printable::displacement_effect(enum displacement d) const {
  switch (d) {
    case displacement::CENTER:
      return find_effect(TextEffect::Kind::CENTER);
    case displacement::RIGHT:
      return find_effect(TextEffect::Kind::RIGHT);
    default:
      throw std::runtime_error("Wrong displacement value");
  }
}

float engine::printable::displacement_spacing(enum displacement d, float width) const {
  switch (d) {
    case displacement::CENTER:
      return parentLocalBounds().width / 2.f - mSystem->margin_horizontal * 1.5f - width / 2.f;
    case displacement::RIGHT:
      return parentLocalBounds().width - 2.5f * mSystem->margin_horizontal - width;
    default:
      throw std::runtime_error("Wrong displacement value");
  }
}

void engine::printable::new_line() const {
  y += mSystem->line_spacing;
  x = mSystem->margin_horizontal;
}

bool engine::printable::end_of_page() const {
  return y >= parentLocalBounds().height - mSystem->margin_vertical;
}

bool engine::printable::end_of_text() const {

}

void engine::printable::updateSelf(sf::Time dt) {
  if (is_current) {
    ensure_updated();
  }
}

void engine::printable::drawSelf(sf::RenderTarget &target, sf::RenderStates states) const {
#ifdef DEBUG
  debug_bounds_vertices.clear();
#endif

  if (sf::VertexBuffer::isAvailable()) {
    if (vertices_buffer.getVertexCount() != vertices.getVertexCount())
      vertices_buffer.create(vertices.getVertexCount());

    if (vertices.getVertexCount() > 0)
      vertices_buffer.update(&vertices[0]);
  }

  auto fz{get_font_size().value_or(mSystem->font_size)};
  states.texture = &mSystem->font->getTexture(fz);

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