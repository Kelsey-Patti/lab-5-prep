#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;
const int FRAMES_PER_ANIMATION = 2*FPS_LIMIT;
const int CIRCLE_START = 50;
const int CIRCLE_END = 700;
const int GRAPH_LEFT = 75;
const int GRAPH_RIGHT = 700;
const int GRAPH_TOP = 400;
const int GRAPH_BOTTOM = 700;

// global tween function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        }
        // ease in out cubic
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){ // asked ChatGPT how to check for pressing number 1 key
            if (keyPressed ->code == sf::Keyboard::Key::Num2){
                tween = [](float a, float b, float t) {
                    float easeInOutCubic = 1 -std::pow(1-t, 4);
                    return (1 - easeInOutCubic) * a + easeInOutCubic* b;
                };
            // ease in sine
            }else if (keyPressed ->code == sf::Keyboard::Key::Num3){
                tween = [](float a, float b, float t) {
                    float easeInSine = 1-std::cos((t*3.141592)/2);
                    return (1 - easeInSine) * a + easeInSine* b;
                };
            // ease out sine
            }else if (keyPressed ->code == sf::Keyboard::Key::Num4){
                tween = [](float a, float b, float t) {
                    float easeOutSine = std::sin((t*3.141592)/2);
                    return (1 - easeOutSine) * a + easeOutSine* b;
                };
            // ease in out cubic
            }else if (keyPressed ->code == sf::Keyboard::Key::Num5){
                tween = [](float a, float b, float t) {
                    float easeInOutCubic = t < 0.5 ? 4 * t * t * t : 1 - std::pow(-2 * t + 2, 3) / 2;
                    return (1 - easeInOutCubic) * a + easeInOutCubic* b;
                };
            
            // ease in expo
            }else if (keyPressed ->code == sf::Keyboard::Key::Num6){
                tween = [](float a, float b, float t) {
                    float easeInExpo = t == 0 ? 0 : std::pow(2, 10 * t - 10);
                    return (1 - easeInExpo) * a + easeInExpo* b;
                };
            
            // ease out bounce
            }else if (keyPressed ->code == sf::Keyboard::Key::Num7){
                tween = [](float a, float b, float t) {
                    float easeOutBounce = t == 0 ? 0 : std::pow(2, 10 * t - 10);
                    const double n1 = 7.5625;
                    const double d1 = 2.75;

                    if (t < 1 / d1) {
                        easeOutBounce= n1 * t * t;
                    } else if (t < 2 / d1) {
                        easeOutBounce= n1 * (t -= 1.5 / d1) * t + 0.75;
                    } else if (t < 2.5 / d1) {
                        easeOutBounce= n1 * (t -= 2.25 / d1) * t + 0.9375;
                    } else {
                        easeOutBounce= n1 * (t -= 2.625 / d1) * t + 0.984375;
                    }
                    
                    return (1 - easeOutBounce) * a + easeOutBounce* b;
                };
            // ease out elastic
            }else if (keyPressed ->code == sf::Keyboard::Key::Num8){
                tween = [](float a, float b, float t) {
                    const double c4 = (2 *3.14159) / 3;

                    float easeOutElastic = t == 0 ?0 : t == 1? 1: std::pow(2, -10 * t) * std::sin((t * 10 - 0.75) * c4) + 1;
                    return (1 - easeOutElastic) * a + easeOutElastic* b;
                };
            // ease in back
            }else if (keyPressed ->code == sf::Keyboard::Key::Num9){
                tween = [](float a, float b, float t) {
                    const double c1 = 1.70158;
                    const double c3 = c1 + 1;

                    float easeInBack = c3 * t * t * t - c1 * t * t;

                    return (1 - easeInBack) * a + easeInBack* b;
                };
            // tween
            }else if (keyPressed ->code == sf::Keyboard::Key::Num1){
                tween = [](float a, float b, float t) {
                    return (1 - t) * a + t* b;
                };
            }
        }
        // ====== ====== ======
        // TODO: (Q2)
        //  implement key presses (1-9) that replace the tween function
        //  with different alternate tween functions.
        //  Functions can be from lecture or from https://easings.net/#
        // ====== ====== ======
    }
}

void render(sf::RenderWindow& window, float t) {
    // Clear with blue background (sky)
    window.clear(sf::Color::Blue);
    // ====== ====== ======
    // TODO: (Q1) Draw circle that moves between
    // the left/right half of the screen.
    // Movement should be governed by the tween function.
    // ====== ====== ======
    sf::CircleShape circle(30.f);
    circle.setFillColor(sf::Color::Magenta); 
    circle.setPosition({tween(CIRCLE_START, CIRCLE_END, t), WINDOW_HEIGHT/3.5});
    window.draw(circle);

    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======
    sf::VertexArray lineX(sf::PrimitiveType::Lines, 2);
    lineX[0].position = sf::Vector2f(GRAPH_LEFT, GRAPH_BOTTOM);
    lineX[1].position = sf::Vector2f(GRAPH_RIGHT, GRAPH_BOTTOM);

    sf::VertexArray lineY(sf::PrimitiveType::Lines, 2);
    lineY[0].position = sf::Vector2f(GRAPH_LEFT, GRAPH_BOTTOM);
    lineY[1].position = sf::Vector2f(GRAPH_LEFT, GRAPH_TOP);
    window.draw(lineX);
    window.draw(lineY);

    sf::VertexArray curve(sf::PrimitiveType::LineStrip);
    for (float t = 0; t <= 1; t+= 0.01f){
        float y = tween(0, 1, t);
        float xCoord = GRAPH_LEFT + t*(GRAPH_RIGHT-GRAPH_LEFT);
        float yCoord = GRAPH_BOTTOM-y*(GRAPH_BOTTOM-GRAPH_TOP);
        sf::Vertex point;
        point.position = {xCoord, yCoord};
        curve.append(point);
    }
    window.draw(curve);

    float tinyRadius = 18.f;
    float y = tween(0, 1, t);
    float xCoord = GRAPH_LEFT + t*(GRAPH_RIGHT-GRAPH_LEFT);
    float yCoord = GRAPH_BOTTOM-y*(GRAPH_BOTTOM-GRAPH_TOP);
    sf::CircleShape tinyCircle(tinyRadius);
    tinyCircle.setFillColor(sf::Color::Yellow);
    tinyCircle.setOrigin({tinyRadius, tinyRadius});
    tinyCircle.setPosition({xCoord, yCoord});
    window.draw(tinyCircle);

    window.display();
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Tween");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        float frame = 0;
        while (window.isOpen()) {
            if (frame == FRAMES_PER_ANIMATION){
                frame = 0;
            }
            frame++;
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            float t = frame/FRAMES_PER_ANIMATION;
            render(window, t);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
