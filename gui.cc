// gui.cc
// Authors: Antoine Devilez & Gaspar Duarte Ribeiro
// Version: 1.0

#include <filesystem>
#include <iostream>
#include "constants.h"
#include "graphic_gui.h"
#include "gui.h"
#include "graphic.h"

using namespace std;

enum Response
{
    CANCEL,
    OPEN_FILE,
    SAVE_FILE
};
enum Buttons
{
    EXIT,
    OPEN,
    SAVE,
    RESTART,
    START,
    STEP
};

constexpr unsigned drawing_size(500);

My_window::My_window(string file_name)
    : main_box(Gtk::Orientation::HORIZONTAL), panel_box(Gtk::Orientation::VERTICAL),
      command_box(Gtk::Orientation::VERTICAL), loop_activated(false),
      buttons({Gtk::Button("exit"), Gtk::Button("open"), Gtk::Button("save"),
               Gtk::Button("restart"), Gtk::Button("start"), Gtk::Button("step")}),
      info_frame("Infos :"), info_text({Gtk::Label("score:"), Gtk::Label("lives:"),
                                        Gtk::Label("bricks:"), Gtk::Label("balls:")})
{
    set_title("Brick Breaker");
    set_child(main_box);
    main_box.append(panel_box);
    main_box.append(drawing);
    panel_box.append(command_box);
    panel_box.append(info_frame);

    set_commands();
    set_key_controller();
    set_mouse_controller();
    set_infos();
    set_drawing();
    if (file_name.empty() == false) {
        Game::ReadResult res = game.read(file_name.c_str());
        if (res == Game::OK)              state = READY;
        else if (res == Game::FILE_NOT_FOUND) state = NO_FILE;
        else                              state = FILE_BAD;
        update_infos();
        drawing.queue_draw();
    }
    update_buttons();
}

void My_window::update_buttons()
{
    bool exit_on    = (state != RUNNING);
    bool open_on    = (state != RUNNING);
    bool save_on    = (state == READY || state == FINISH);
    bool restart_on = (state == FILE_BAD || state == READY || state == FINISH);
    bool start_on   = (state == READY || state == RUNNING);
    bool step_on    = (state == READY);

    buttons[EXIT].set_sensitive(exit_on);
    buttons[OPEN].set_sensitive(open_on);
    buttons[SAVE].set_sensitive(save_on);
    buttons[RESTART].set_sensitive(restart_on);
    buttons[START].set_sensitive(start_on);
    buttons[STEP].set_sensitive(step_on);
    buttons[START].set_label(state == RUNNING ? "stop" : "start");
}
void My_window::set_commands()
{
    for (auto &button : buttons)
    {
        command_box.append(button);
        command_box.set_size_request(120, -1);
        button.set_margin(1);
    }

    buttons[EXIT].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::exit_clicked));
    buttons[OPEN].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::open_clicked));
    buttons[SAVE].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::save_clicked));
    buttons[RESTART].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::restart_clicked));
    buttons[START].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::start_clicked));
    buttons[STEP].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::step_clicked));
}

void My_window::exit_clicked()
{
    hide();
}
void My_window::open_clicked()
{
    auto dialog = new Gtk::FileChooserDialog("Choose a text file",
                                             Gtk::FileChooserDialog::Action::OPEN);
    set_dialog(dialog);
}
void My_window::save_clicked()
{
    auto dialog = new Gtk::FileChooserDialog("Choose a text file",
                                             Gtk::FileChooserDialog::Action::SAVE);
    set_dialog(dialog);
}
void My_window::restart_clicked()
{
    if (state == NO_FILE) return;
    if (loop_activated) return;
    if (game.restart()) state = READY;
    else                state = FILE_BAD;
    update_infos();
    update_buttons();
    drawing.queue_draw();
}
void My_window::start_clicked()
{
    if (state != READY && state != RUNNING) return;
    if (loop_activated)
    {
        loop_conn.disconnect();
        loop_activated = false;
        state = READY;
    }
    else
    {
        loop_conn =
            Glib::signal_timeout().connect(sigc::mem_fun(*this, &My_window::loop), dt);
        loop_activated = true;
        state = RUNNING;
    }
    update_buttons();
}
void My_window::step_clicked()
{
    if (state != READY) return;
    game.step();
    update_infos();
    drawing.queue_draw();
}
void My_window::set_key_controller()
{
    auto contr = Gtk::EventControllerKey::create();
    contr->signal_key_pressed().connect(sigc::mem_fun(*this, &My_window::key_pressed),
                                        false);
    add_controller(contr);
}
bool My_window::key_pressed(guint keyval, guint keycode, Gdk::ModifierType state)
{
    switch (keyval)
    {
    case '1':
        step_clicked();
        return true;
    case 's':
        start_clicked();
        return true;
    case 'r':
        restart_clicked();
        return true;
    default:
        break;
    }
    return false;
}

void My_window::set_dialog(Gtk::FileChooserDialog *dialog)
{
    dialog->set_modal(true);
    dialog->set_transient_for(*this);
    dialog->set_select_multiple(false);
    dialog->signal_response().connect(
        sigc::bind(sigc::mem_fun(*this, &My_window::dialog_response), dialog));

    dialog->add_button("_Cancel", CANCEL);
    switch (dialog->get_action())
    {
    case Gtk::FileChooserDialog::Action::OPEN:
        dialog->add_button("_Open", OPEN_FILE);
        break;
    case Gtk::FileChooserDialog::Action::SAVE:
        dialog->add_button("_Save", SAVE_FILE);
        break;
    default:
        break;
    }

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_pattern("*.txt");
    dialog->add_filter(filter_text);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog->add_filter(filter_any);

    dialog->show();
}
void My_window::dialog_response(int response, Gtk::FileChooserDialog *dialog)
{
    filesystem::path file_name = "";
    if (dialog->get_file())
    {
        file_name = dialog->get_file()->get_path();
        if (file_name.extension() != ".txt")
        {
            file_name = "";
        }
    }
    switch (response)
    {
    case CANCEL:
        dialog->hide();
        break;
    case OPEN_FILE:
        if (file_name != "")
        {
            Game::ReadResult res = game.read(file_name.string().c_str());
            if (res == Game::OK)              state = READY;
            else if (res == Game::FILE_NOT_FOUND) state = NO_FILE;
            else                              state = FILE_BAD;
            update_infos();
            update_buttons();
            drawing.queue_draw();
            dialog->hide();
        }
        break;
    case SAVE_FILE:
        if (file_name != "")
        {
            game.save(file_name.string());
            dialog->hide();
        }
        break;
    default:
        break;
    }
}

bool My_window::loop() {
    state = READY;
    if (game.get_status() == Game::ONGOING) {
        game.step();
        update_infos();
        drawing.queue_draw();
        state = RUNNING;
        return true;
    }
    else if (game.get_status() == Game::STOPPED) state = FINISH;
    loop_activated = false;
    update_buttons();
    update_infos();
    drawing.queue_draw();
    return false;
}

void My_window::set_infos()
{
    info_frame.set_child(info_grid);
    info_grid.set_column_homogeneous(true);
    for (size_t i(0); i < info_text.size(); ++i)
    {
        info_grid.attach(info_text[i], 0, i, 1, 1);
        info_grid.attach(info_value[i], 1, i, 1, 1);
        info_text[i].set_halign(Gtk::Align::START);
        info_value[i].set_halign(Gtk::Align::END);
        info_text[i].set_margin(3);
        info_value[i].set_margin(3);
    }
}

void My_window::update_infos()
{
    info_value[0].set_text(to_string(game.get_score()));
    info_value[1].set_text(to_string(game.get_nb_lives()));
    info_value[2].set_text(to_string(game.get_nb_bricks()));
    info_value[3].set_text(to_string(game.get_nb_balls()));
}

void My_window::set_drawing()
{
    drawing.set_content_width(drawing_size);
    drawing.set_content_height(drawing_size);
    drawing.set_expand();
    drawing.set_draw_func(sigc::mem_fun(*this, &My_window::on_draw));
}
void My_window::on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height)
{
    graphic_set_context(cr);
    double side(min(width, height));
    cr->translate((width - side) / 2, (height + side) / 2);
    cr->scale(side / (arena_size), -side / (arena_size));

    // TODO: draw the game
    if (state == NO_FILE || state == FILE_BAD) {
        return;
    }
    for (const auto& brick : game.get_bricks()) {
        brick->draw();
    }
    for (const auto& ball : game.get_balls()) {
        ball.draw();
    }
    game.get_paddle().draw();

    set_color(GREY);
    cr->rectangle(0, 0, arena_size, arena_size);
    cr->stroke();
}

void My_window::set_mouse_controller()
{
    auto left_click = Gtk::GestureClick::create();
    auto move = Gtk::EventControllerMotion::create();

    left_click->set_button(GDK_BUTTON_PRIMARY);

    left_click->signal_pressed().connect(
        sigc::mem_fun(*this, &My_window::on_drawing_left_click));
    move->signal_motion().connect(sigc::mem_fun(*this, &My_window::on_drawing_move));

    drawing.add_controller(left_click);
    drawing.add_controller(move);
}


void My_window::on_drawing_left_click(int n_press, double x, double y)
{
    if (n_press == 1 && game.get_nb_lives() > 0 and game.get_nb_balls() == 0) {
        game.new_ball();
        game.consume_life();
        update_infos();
    }
    drawing.queue_draw(); // TODO
}


void My_window::on_drawing_move(double x, double y)
{
    double side = min(drawing.get_width(), drawing.get_height());
    double offset = (drawing.get_width() - side) / 2.0;
    double x_game = (x - offset) * arena_size / side;
    game.set_target_paddle(x_game);
}
