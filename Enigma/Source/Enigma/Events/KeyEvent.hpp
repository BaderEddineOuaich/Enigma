#pragma once
#ifndef ENIGMA_KEY_EVENT_H
#define ENIGMA_KEY_EVENT_H

#include <Enigma/Core/Core.hpp>
#include <Enigma/Core/Types.hpp>
#include <Enigma/Input/Keyboard/KeyCodes.hpp>
#include "Event.hpp"
#include <sstream>

NS_ENIGMA_BEGIN

class KeyEvent : public Event
{
public:
	const KeyCode& GetKeyCode() const noexcept { return m_key_code; }

	EVENT_CLASS_CATEGORY(EC_KEYBOARD | EC_INPUT)
	
protected:
	explicit KeyEvent(const KeyCode keyCode)
		:
		m_key_code(keyCode)
	{}

	KeyCode m_key_code;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(const KeyCode keycode, const ui16 repeat_count)
		:
		KeyEvent(keycode),
		m_repeat_count(repeat_count) {}

	const ui16& GetRepeatCount() const noexcept { return m_repeat_count; }

	String ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_key_code << " (" << m_repeat_count << " repeats)";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KEY_PRESSED)

private:
	ui16 m_repeat_count;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	explicit KeyReleasedEvent(const KeyCode keyCode)
		:
		KeyEvent(keyCode) 
	{}

	String ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_key_code;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KEY_RELEASED)
};

class KeyTypedEvent : public KeyEvent
{
public:
	explicit KeyTypedEvent(const KeyCode keyCode)
		:
		KeyEvent(keyCode) {}

	String ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << m_key_code;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KEY_TYPED)
};


NS_ENIGMA_END

#endif // !ENIGMA_KEY_EVENT_H
