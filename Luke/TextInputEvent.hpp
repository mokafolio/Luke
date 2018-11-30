#ifndef LUKE_TEXTINPUTEVENT_HPP
#define LUKE_TEXTINPUTEVENT_HPP

#include <Luke/Constants.hpp>
#include <Stick/Event.hpp>

namespace luke
{
class TextInputEvent : public stick::EventT<TextInputEvent, LukeEventCategory>
{
  public:
    TextInputEvent(const char * _utf8);

    const stick::String & text() const;

  private:
    stick::String m_utf8;
};
} // namespace luke

#endif // LUKE_TEXTINPUTEVENT_HPP
