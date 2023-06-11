#pragma once
#include "Rigidbody.h"

class Circle : public Rigidbody
{
public:
    Circle(glm::vec2 position, glm::vec2 velocity,
        float mass, float radius, glm::vec4 colour);
    ~Circle();

	virtual void Draw(float alpha);

    float GetRadius() { return m_radius; }
    glm::vec4 GetColor() { return m_color; }
    void SetPocket(bool state) { m_isPocket = state; }
    void SetStripe(bool stripe) { m_isStripe = stripe; }
    bool GetStripe() { return m_isStripe; }
    void SetColor(glm::vec4 color) { m_color = color; }
    void SetSunkBallCheck(bool playerCheck) { m_isPlayer1s = playerCheck; }
    bool GetSunkBallCheck() { return m_isPlayer1s; }
   

protected:
    float m_radius;
    glm::vec4 m_color;
    bool m_isPocket = false;
    bool m_isStripe = false;
    bool m_isPlayer1s = false;

};

