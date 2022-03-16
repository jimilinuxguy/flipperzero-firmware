#include <furi.h>
#include <dialogs/dialogs.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/empty_screen.h>
#include <m-string.h>
#include <furi_hal_version.h>

typedef DialogMessageButton (*AboutDialogScreen)(DialogsApp* dialogs, DialogMessage* message);

// Screen 1
static DialogMessageButton product_screen(DialogsApp* dialogs, DialogMessage* message) {
    DialogMessageButton result;

    const char* screen_header = "Welcome to OnlyFlipper\n"
                                "A platform for hackers!\n";
    const char* screen_text = "What you are about to see\n"
                              "may shock you and others!";

    dialog_message_set_header(message, screen_header, 0, 0, AlignLeft, AlignTop);
    dialog_message_set_text(message, screen_text, 0, 26, AlignLeft, AlignTop);
    result = dialog_message_show(dialogs, message);
    dialog_message_set_header(message, NULL, 0, 0, AlignLeft, AlignTop);
    dialog_message_set_text(message, NULL, 0, 0, AlignLeft, AlignTop);

    return result;
}

// Screen 4
static DialogMessageButton icon1_screen(DialogsApp* dialogs, DialogMessage* message) {
    DialogMessageButton result;

    dialog_message_set_icon(message, &I_OF1_119x50, 12, 12);
    result = dialog_message_show(dialogs, message);
    dialog_message_set_icon(message, NULL, 0, 0);

    return result;
}

// Screen 5
static DialogMessageButton icon2_screen(DialogsApp* dialogs, DialogMessage* message) {
    DialogMessageButton result;

    dialog_message_set_icon(message, &I_OF1_119x50, 12, 12);
    result = dialog_message_show(dialogs, message);
    dialog_message_set_icon(message, NULL, 0, 0);

    return result;
}

const AboutDialogScreen about_screens[] = {
    product_screen,
    icon1_screen,
    icon2_screen,};

const size_t about_screens_count = sizeof(about_screens) / sizeof(AboutDialogScreen);

int32_t only_flipper_app(void* p) {
    DialogsApp* dialogs = furi_record_open("dialogs");
    DialogMessage* message = dialog_message_alloc();

    Gui* gui = furi_record_open("gui");
    ViewDispatcher* view_dispatcher = view_dispatcher_alloc();
    EmptyScreen* empty_screen = empty_screen_alloc();
    const uint32_t empty_screen_index = 0;

    size_t screen_index = 0;
    DialogMessageButton screen_result;

    // draw empty screen to prevent menu flickering
    view_dispatcher_add_view(
        view_dispatcher, empty_screen_index, empty_screen_get_view(empty_screen));
    view_dispatcher_attach_to_gui(view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    view_dispatcher_switch_to_view(view_dispatcher, empty_screen_index);

    while(1) {
        if(screen_index >= about_screens_count - 1) {
            dialog_message_set_buttons(message, "Back", NULL, NULL);
        } else {
            dialog_message_set_buttons(message, "Back", NULL, "Next");
        }

        screen_result = about_screens[screen_index](dialogs, message);

        if(screen_result == DialogMessageButtonLeft) {
            if(screen_index <= 0) {
                break;
            } else {
                screen_index--;
            }
        } else if(screen_result == DialogMessageButtonRight) {
            if(screen_index < about_screens_count) {
                screen_index++;
            }
        } else if(screen_result == DialogMessageButtonBack) {
            break;
        }
    }

    dialog_message_free(message);
    furi_record_close("dialogs");

    view_dispatcher_remove_view(view_dispatcher, empty_screen_index);
    view_dispatcher_free(view_dispatcher);
    empty_screen_free(empty_screen);
    furi_record_close("gui");

    return 0;
}
