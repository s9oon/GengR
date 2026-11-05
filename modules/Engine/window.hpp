namespace GengR::Engine {
    class Window {
    public:
        Window(const char* title, int width, int height); // throws on failure
        ~Window();

        void draw();

    private:
        void drawSqaure();
    };
}