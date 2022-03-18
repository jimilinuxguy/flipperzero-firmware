#include <furi.h>
#include <furi_hal.h>

#include <gui/gui.h>
#include <input/input.h>
#include <notification/notification_messages.h>

void vibrator_draw_callback(Canvas* canvas, void* ctx) {
    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "Vibrator w/ Patterns");
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 22, "Ok ---- Off");
    canvas_draw_str(canvas, 2, 34, "Left -- Slow");
    canvas_draw_str(canvas, 2, 46, "Up ---- Med");
    canvas_draw_str(canvas, 2, 58, "Right - Fast");
    canvas_draw_str(canvas, 2, 70, "Down -- Continuous");
}

void vibrator_input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(ctx);
    osMessageQueueId_t event_queue = ctx;
    osMessageQueuePut(event_queue, input_event, 0, osWaitForever);
}

int32_t vibrator_app(void* p) {
    osMessageQueueId_t event_queue = osMessageQueueNew(8, sizeof(InputEvent), NULL);

    // Configure view port
    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, vibrator_draw_callback, NULL);
    view_port_input_callback_set(view_port, vibrator_input_callback, event_queue);

    // Register view port in GUI
    Gui* gui = furi_record_open("gui");
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    NotificationApp* notification = furi_record_open("notification");
    InputEvent event;
    int status = 0;

    while(osMessageQueueGet(event_queue, &event, NULL, osWaitForever) == osOK) {

        // Check for existing status
        // +==========================+
        // | 0 - stopped              |
        // | 1 - low                  |
        // | 2 - med                  |
        // | 3 - high                 |
        // | 4 - continuous           |
        // +==========================+

        if (status == 0) {
            // keep waiting! 
        } else if (status == 1) {
            // slow
            notification_message(notification, &sequence_set_vibro_on);
            osDelay(900);
        } else if (status == 2) {
            // med
            notification_message(notification, &sequence_set_vibro_on);
            osDelay(600);
        } else if (status == 3) {
            // fast
            notification_message(notification, &sequence_set_vibro_on);
            osDelay(300);
        } else if (status == 4) {
            // did this already
        } else {
            // This should never happen, as status will always be defined
        }
        
        // Exit
        if(event.key == InputKeyBack) {
            notification_message(notification, &sequence_reset_vibro);
            break;
        }

        // Toggle Off
        if(event.key == InputKeyOk) {
            mode = 0
            notification_message(notification, &sequence_reset_vibro);
        }

        // Slow (Left)
        if(event.key == InputKeyLeft) {
            int status = 1
        }

        // Med (Up)
        if(event.key == InputKeyUp) {
            int status = 2
        }

        // Fast (Right)
        if(event.key == InputKeyRight) {
            int status = 3
        }

        // Continuous (Down)
        if(event.key == InputKeyDown) {
            if(event.type == InputTypeShort) {
                bool status = 4
                notification_message(notification, &sequence_set_vibro_on);
            }
        }
    }

    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    osMessageQueueDelete(event_queue);

    furi_record_close("notification");
    furi_record_close("gui");

    return 0;
}