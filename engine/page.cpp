//
// Created by marcin on 7/19/19.
//

#include "page.hpp"

engine::page::page(
    SystemPtr sys_ptr,
    audio_system_ptr audio_ptr,
    story_ptr story_ptr
) :
    GameObject{std::move(sys_ptr)},
    Entity{},
    story{std::move(story_ptr)},
    audio{std::move(audio_ptr)},
    printable_refs{},
    current_printable{},
    vertices{sf::Triangles},
    vertices_buffer{sf::Triangles, sf::VertexBuffer::Static},
    debug_bounds_vertices{sf::Lines},
    font_texture_vertices{sf::Triangles},
    needs_advance{true},
    needs_update{true},
    needs_redraw{true},
    needs_first_preprocess{true},
    x{mSystem->margin_horizontal},
    y{mSystem->margin_vertical + static_cast<float>(mSystem->font_size)},
    min_x{},
    min_y{},
    max_x{},
    max_y{} {
  attachAnimation<translate_vertical>(
      LINE_SHIFT,
      translate_vertical::from(0.f)
          .to(-mSystem->line_spacing)
          .during(30)
          .via(tweeny::easing::quadraticInOut)
          .on_step([&] {
            //redraw();
          })
  );

  story->set_store(store());
  story->init();
}

bool engine::page::can_advance() const {
  return needs_advance;
}

void engine::page::advance() {
  if (std::next(current_printable) == std::end(printable_refs)) {
    needs_advance = false;
  } else {
    current_printable = std::next(current_printable);
    needs_update = true;
  }
}

void engine::page::input() {
  if (const auto &mp{mSystem->mouse_position()}; globalBounds().contains(mp)) {
    apply_mouse_hover(mp);
    if (mSystem->mouse_click_available()) {
      apply_mouse_click(mSystem->mouse_click_position());
    } else {
      //redraw();
    }
  }
}

void engine::page::updateSelf(sf::Time dt) {
  if (can_advance()) {
    if (hasElapsed(sf::milliseconds(200))) {
      advance();
    }
  } else {
    input();
  }
}

void engine::page::drawSelf(sf::RenderTarget &target, sf::RenderStates states) const {

}

engine::printable_store engine::page::store() {
  return {
      [this](printable_ptr &&ptr) {
        printable_refs.push_back(std::ref(*ptr));
        addChild(std::move(ptr));
      },
      [this](printable_ptr &&ptr) {
        printable_refs.push_back(std::ref(*ptr));
        addChild(std::move(ptr));
      },
      [this](size_t n) {
        if (n >= printable_refs.size()) {
          throw std::runtime_error("Trying to truncate the entire printable store");
        }

        auto pid{pointer(current_printable).id()};
        for (size_t i = printable_refs.size() - 1; i >= printable_refs.size() - n; --i) {
          removeChild(printable_refs[i].get());
        }
        printable_refs.resize(printable_refs.size() - n);
        current_printable = find_printable(pid);

        if (current_printable == std::end(printable_refs)) {
          current_printable = gen::last(printable_refs);
        }

        y = rect(current_printable).top;
        x = rect(current_printable).left;
        max_y = std::min(max_y, rect(current_printable).top + rect(current_printable).height);
        max_x = std::min(max_x, rect(current_printable).left + rect(current_printable).width);
      },
      [this]() {
        current_printable = std::next(current_printable);

        needs_advance = true;
        needs_update = true;
      },
      [this]() {
        //redraw();
      }
  };
}

void engine::page::apply_mouse_hover(const sf::Vector2f &cursor) {
  bool hovering{};
  for (auto &printable : printable_refs) {
    if (printable.get().interactive()) {
      auto t{printable.get().globalBounds()};
      if (parentIntersects(t) && t.contains(cursor)) {
        printable.get().on_hover_start();
        hovering = true;
      } else {
        printable.get().on_hover_end();
      }

      needs_redraw = true;
    }
  }

  mSystem->set_cursor(hovering
                     ? System::Cursor::HAND
                     : System::Cursor::ARROW);
}

void engine::page::apply_mouse_click(const sf::Vector2f &cursor) {
  for (auto &printable : printable_refs) {
    if (printable.get().interactive()) {
      auto t{printable.get().globalBounds()};
      if (parentIntersects(t) && t.contains(cursor)) {
        story->act(printable.get().on_click());
        mSystem->set_cursor(System::Cursor::ARROW);
        break;
      }
    }
  }
}

void engine::page::new_line() const {
  y += mSystem->line_spacing;
  x = mSystem->margin_horizontal;
  if (end_of_page()) {
    get_animation(LINE_SHIFT).start();
  }
}

bool engine::page::end_of_page() const {
  return y >= parentLocalBounds().height - mSystem->margin_vertical;
}

void engine::page::draw_page_outline() const {
  auto root_trans{globalBounds()};
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