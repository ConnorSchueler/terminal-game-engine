#ifndef ENGINE_HPP
#define ENGINE_HPP

class Engine{
    private:
        bool is_running;
        int x_grid;
        int y_grid;
        int x_player;
        int y_player;

        void render();
        void input();
        void update();


    public:
        Engine ();
        void run();
};

#endif