#include "input_module.h"

HRESULT InputModule::Init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight) {
    HRESULT result = S_OK;

    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);

    if (SUCCEEDED(result)) {
        result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
    }

    if (SUCCEEDED(result)) {
        result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
    }

    if (SUCCEEDED(result)) {
        result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    }

    if (SUCCEEDED(result)) {
        result = m_keyboard->Acquire();
    }

    if (SUCCEEDED(result)) {
        result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
    }

    if (SUCCEEDED(result)) {
        result = m_mouse->SetDataFormat(&c_dfDIMouse);
    }

    if (SUCCEEDED(result)) {
        result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    }

    if (SUCCEEDED(result)) {
        result = m_mouse->Acquire();
    }

    return result;

}

void InputModule::Resize(int screenWidth, int screenHeight) {
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
}

void InputModule::Realese() {
    if (m_mouse) {
        m_mouse->Unacquire();
        m_mouse->Release();
        m_mouse = nullptr;
    }

    if (m_keyboard) {
        m_keyboard->Unacquire();
        m_keyboard->Release();
        m_keyboard = 0;
    }

    if (m_directInput) {
        m_directInput->Release();
        m_directInput = 0;
    }
}

bool InputModule::Frame() {
    bool result;

    result = ReadKeyboard();
    if (!result)
        return false;

    result = ReadMouse();
    if (!result)
        return false;

    return true;
}

bool InputModule::ReadKeyboard() {
    HRESULT result;

    result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
    if (FAILED(result)) {
        if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
            m_keyboard->Acquire();
        else
            return false;
    }

    return true;
}


bool InputModule::ReadMouse() {
    HRESULT result;

    result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
    if (FAILED(result)) {
        if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
            m_mouse->Acquire();
        else
            return false;
    }

    return true;
}

XMFLOAT3 InputModule::IsMouseUsed() {
    if (m_mouseState.rgbButtons[0] || m_mouseState.rgbButtons[1] || m_mouseState.rgbButtons[2] & 0x80)
        return XMFLOAT3((float)m_mouseState.lX, (float)m_mouseState.lY, (float)m_mouseState.lZ);

    return XMFLOAT3(0.0f, 0.0f, 0.0f);
};
