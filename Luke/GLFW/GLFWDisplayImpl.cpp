#include <Luke/GLFW/GLFWDisplayImpl.hpp>
#include <Luke/GLFW/GLFWInitializer.hpp>

namespace luke
{
    namespace detail
    {
        using namespace stick;
        using namespace crunch;

        DisplayImpl::DisplayImpl() :
            m_glfwMonitor(NULL)
        {

        }

        DisplayImpl::~DisplayImpl()
        {

        }

        DisplayImplUniquePtr DisplayImpl::clone() const
        {
            auto ret = makeUnique<DisplayImpl>(defaultAllocator());
            ret->m_glfwMonitor = m_glfwMonitor;
            return ret;
        }

        Error DisplayImpl::setDisplayMode(const DisplayMode & _mode)
        {
            STICK_ASSERT(isValid());
            //THERE IS NO GLFW WAY TO CHANGE THE DISPLAY MODE UNLESS
            //YOU ARE GOING FULLSCREEN WITH A WINDOW
            return Error(ec::Unsupported, "GLFW only allows you to change the display mode when entering fullscreen with a window", STICK_FILE, STICK_LINE);
        }

        DisplayMode DisplayImpl::currentDisplayMode() const
        {
            STICK_ASSERT(isValid());
            const GLFWvidmode * mode = glfwGetVideoMode(m_glfwMonitor);
            return DisplayMode(mode->width, mode->height, mode->redBits, mode->greenBits, mode->blueBits, mode->refreshRate);
        }

        bool DisplayImpl::isValid() const
        {
            return m_glfwMonitor;
        }

        DisplayModeArray DisplayImpl::displayModes() const
        {
            STICK_ASSERT(isValid());
            DisplayModeArray ret;
            int count;
            const GLFWvidmode * modes = glfwGetVideoModes(m_glfwMonitor, &count);
            for (Size i = 0; i < count; ++i)
            {
                ret.append(DisplayMode(modes[i].width, modes[i].height, modes[i].redBits, modes[i].greenBits, modes[i].blueBits, modes[i].refreshRate));
            }
            return ret;
        }

        Display DisplayImpl::mainDisplay()
        {
            GLFWInitializer::instance();

            GLFWmonitor * primary = glfwGetPrimaryMonitor();
            if (primary)
            {
                Display ret;
                ret.m_pimpl->m_glfwMonitor = primary;
                return ret;
            }
            return Display();
        }

        Vec2f DisplayImpl::position() const
        {
            STICK_ASSERT(isValid());
            int x, y;
            glfwGetMonitorPos (m_glfwMonitor, &x, &y);
            return Vec2f(x, y);
        }

        DisplayArray DisplayImpl::displays()
        {
            GLFWInitializer::instance();

            DisplayArray ret;

            int count;
            GLFWmonitor ** monitors = glfwGetMonitors(&count);
            for (Size i = 0; i < count; ++i)
            {
                Display d;
                d.m_pimpl->m_glfwMonitor = monitors[i];
                ret.append(std::move(d));
            }

            return ret;
        }
    }
}
