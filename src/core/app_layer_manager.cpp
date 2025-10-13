#include "include/app_layer_manager.h"

AppLayerManager::AppLayerManager(): layer_operation_queue{} {
    this->layer_operation_count = 0;
}

void AppLayerManager::update() {
    for (int i = 0; i < layer_operation_count; i++) {
        LayerOpp new_op = layer_operation_queue[i];
        if (new_op.type == LayerOpp::LAYER_POP) {
            if (app_layers.empty()) continue;
            AppLayer *old = app_layers.back();
            app_layers.pop_back();
            delete old;
        } else if (new_op.type == LayerOpp::LAYER_PUSH) {
            app_layers.push_back(new_op.layer);
        }
    }
    layer_operation_count = 0;
}


void AppLayerManager::tick(VoxelGame *game) {
    for (int i = app_layers.size() - 1; i >= 0; i--) {
        if (!app_layers[i]->tick(game)) {
            break;
        }
    }
}

void AppLayerManager::render(VoxelGame *game, float deltaTime) {
    for (int i = 0; i < app_layers.size(); i++) {
        app_layers[i]->render(game, deltaTime);
    }
}

void AppLayerManager::resize(int width, int height) {
    for (int i = 0; i < app_layers.size(); i++) {
        app_layers[i]->resize(width, height);
    }
}

void AppLayerManager::pushAppLayer(AppLayer *layer)
{
    if (this->layer_operation_count == MAX_APP_LAYER_OPS) {return;}
    this->layer_operation_queue[this->layer_operation_count++] = {
        .type = LayerOpp::LAYER_PUSH,
        .layer = layer
    };
}

void AppLayerManager::popAppLayer()
{
    if (this->layer_operation_count == MAX_APP_LAYER_OPS) {return;}
    this->layer_operation_queue[this->layer_operation_count++] = {
        .type = LayerOpp::LAYER_POP
    };
}

bool AppLayerManager::isEmpty() const
{
    return app_layers.empty();
}
