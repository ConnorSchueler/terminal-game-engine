#ifndef ENGINE_HPP
#define ENGINE_HPP

class Engine{
    protected:
        bool is_running;
        int x_grid;
        int y_grid;
        int x_player;
        int y_player;
        int score;
        int x_food;
        int y_food;

        virtual void render();
        virtual void input();
        virtual void update();
        void spawnFood();


    public:
        Engine ();
        virtual ~Engine();

        void run();
};

#endif