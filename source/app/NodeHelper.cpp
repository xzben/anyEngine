#include "NodeHelper.h"

#include "components/Camera.h"
#include "render/2d/Sprite.h"
BEGIN_NS_SCENCE_GRAPH

Node* NodeHelper::createSprite() {
    Node* node = new Node();
    node->setName("Sprite");
    node->addComponent<Sprite>("Sprite");
    return node;
}

Node* NodeHelper::createCamera() {
    Node* node = new Node();
    node->setName("Camera");
    node->addComponent<Camera>("Camera");
    return node;
}

END_NS_SCENCE_GRAPH