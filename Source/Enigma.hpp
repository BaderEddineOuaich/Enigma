#pragma once
#ifndef ENIGMA_H
#define ENIGMA_H

//Core
#include <Core/Core.hpp>
#include <Application/Application.hpp>
//

//Window
#include <Window/Window.hpp>
//

//Enigma Config Loader
#include <Config/Config.hpp>
//


//System Dialogs, Notification, Clipboard
#include <System/Dialogs/MessageBox.hpp>
#include <System/Dialogs/OpenFileDialog.hpp>
#include <System/Dialogs/SaveFileDialog.hpp>
#include <System/Dialogs/SelectFolderDialog.hpp>
#include <System/Notification/Notification.hpp>
#include <System/Clipboard/Clipboard.hpp>
//

//Events
#include <Events/Event.hpp>
#include <Events/EventDispatcher.hpp>
#include <Events/ApplicationEvent.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Events/JoystickEvent.hpp>
//

//Input
#include <Input/Input.hpp>
#include <Input/Keyboard/KeyCodes.hpp>
#include <Input/Mouse/MouseCodes.hpp>
//

//Logging 
#include <Logger/Logger.hpp>
//


//GUI 
#include <GUI/ImGuiRenderer.hpp>
#include <GUI/ImGuiWidgets.hpp>
//

//Scene
#include <Scenes/Scene.hpp>
//

//Utility 
#include <Utility/Random.hpp>
#include <Utility/Base64.hpp>
#include <Utility/FileUtils.hpp>
#include <Utility/StringUtils.hpp>
#include <Utility/OpenGLUtils.hpp>
#include <Utility/CryptoPPUtils.hpp>
#include <Utility/DialogUtils.hpp>
#include <Utility/GZip.hpp>
//


//Analytics: Hardware info RAM, CPU
#include <Analytics/Hardware/RAM/RAMInfo.hpp>
#include <Analytics/Hardware/CPU/CPUInfo.hpp>
//

//Enigma Command line interface
#include <CLI/CLI.hpp>
//

//Networking
#include <Networking/CheckForUpdates.hpp>
//

//Database
#include <Database/Database.hpp>
//

#endif // !ENIGMA_H
