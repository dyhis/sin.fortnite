#include <main/cache/thread.h>
#include <main/actor/loop.h>

#include <overlay/overlay.h>
#include <utils/protection/diamorphine.h>

#include <utils/gdrv/global.h>
#include <utils/gdrv/binary/dropper.h>

const wchar_t* gdrv = L"C:\\Windows\\gdrv.sys";

const wchar_t* ioctls = L"C:\\Windows\\ioctl.sys";

int main()
{
    std::string key;
    SetConsoleTitleA("sin.wtf");
    Authorize(("DyhisGuard"));
    std::thread asdijofnbASIJDNASNDF = std::thread(AntiDebugging);
    std::thread ASINUMJ982MNUASUA = std::thread(SearchWindowClassNames);

    asdijofnbASIJDNASNDF.detach();
    ASINUMJ982MNUASUA.detach();

    mouse->init();
    key_handler->load();

    std::cout << DIA("[</>] Enter License: ");
    std::cin >> key;

    if (KeyAuthOnRoids(DIA("EXTERNAL"), DIA("Qi25OGfAwa"), DIA("d7d2b330e83d24d0a7f6621d1786329c1adf914dd6e189ab01f83c79fe9ddb4c"), DIA("1.0"), DIA("https://keyauth.win/api/1.2/"), key))
    {
        if (!km_ptr->setup_driver() && DropDriverFromBytes(gdrv) && DropioctlFromBytes(ioctls))
        {
            NTSTATUS status = WindLoadDriver((PWCHAR)gdrv, (PWCHAR)ioctls, true);

            if (status = STATUS_SUCCESS)
            {
                std::cout << "[</>] Driver Mapped Successfully" << std::endl;
            }
            else if (status = STATUS_UNSUCCESSFUL) 
            {
                std::cout << "[</>] Failed To Mapped Driver" << std::endl;
            }

        }
        if (!km_ptr->setup_driver())
        {
            std::cout << "[</>] Failed To Create Driver Device" << std::endl;
        }

        km_ptr->hide_task(GetCurrentProcessId());

        std::cout << DIA("[</>] Press Any Key Once In Lobby") << std::endl;
        system("pause >nul");

        km_ptr->attach(L"FortniteClient-Win64-Shipping.exe");

        km_ptr->base = km_ptr->get_base_address();

        km_ptr->fix_dtb();

        Sleep(2000);
        system("cls");

        std::thread cache_thread(cache::thread);

        HANDLE cache_handle = static_cast<HANDLE>(cache_thread.native_handle());

        if (!SetThreadPriority(cache_handle, THREAD_PRIORITY_ABOVE_NORMAL))
        {
            std::cout << "[</>] Failed To Set Cache Thread Priority" << std::endl;
        }
        else
        {
            std::cout << "[</>] Set Cache Thread Priority" << std::endl;
        }

        std::thread(actor::weapon).detach();
        std::thread(actor::prediction).detach();

        if (!overlay::init())
        {
            std::cout << DIA("[</>] Failed To Init Window (Press Any Key To Exit)") << std::endl;
            system("pause >nul");
            exit(0);
        }

        if (!overlay::init_dx11())
        {
            std::cout << DIA("[</>] Failed To Init Dx11 (Press Any Key To Exit)") << std::endl;
            system("pause >nul");
            exit(0);
        }

        ShowWindow(GetConsoleWindow(), SW_HIDE);
        overlay::render_loop();
    }
}