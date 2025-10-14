#pragma once
#include <vector>

#include "app_layer.h"

#define MAX_APP_LAYER_OPS 16

class AppLayerManager {
    // Layer Opperation Struct
    struct LayerOpp {
        enum {
            LAYER_PUSH,
            LAYER_POP
        } type;
        union {
            // Tagged union with just a single pointer in it, how scandalously inefficient, well tough
            struct { AppLayer *layer; };
        };
    };

    // Simple staticly sized array for layer operations, even this is probably overkill
    LayerOpp layer_operation_queue[MAX_APP_LAYER_OPS];
    int layer_operation_count = 0;

    // App layer stack
    std::vector<AppLayer*> app_layers;

public:
    AppLayerManager();

    void update();
    void tick();
    void render(float deltaTime);
    void resize(int width, int height);

    /// Queues up an app layer to be pushed onto the layer stack at the end of the frame
    /// *layer: The layer to push
    void pushAppLayer(AppLayer *layer);
    /// Queues up a pop opperation on the app layer stack at the end of the frame
    void popAppLayer();

    bool isEmpty() const;

};
