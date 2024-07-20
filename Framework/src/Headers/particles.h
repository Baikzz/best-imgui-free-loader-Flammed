#pragma once
#include "main.h"
#include <vector>

struct Particle
{
    ImVec2 position;
    ImVec2 velocity;
    float life;
    float size;
    ImU32 color;
};

std::vector<Particle> particles;
const int MAX_PARTICLES = 100;

void InitializeParticles()
{
    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        Particle particle;
        particle.position = ImVec2(rand() % 700, rand() % 475);
        particle.velocity = ImVec2(0, -(rand() % 20 / 100.0f + 0.2f));
        particle.life = 1.0f;
        particle.size = (rand() % 3 + 1) * 0.5f;
        particle.color = IM_COL32(255, 255, 255, 96);
        particles.push_back(particle);
    }
}

void UpdateParticles(float deltaTime)
{
    for (int i = 0; i < particles.size(); ++i)
    {
        particles[i].position.y += particles[i].velocity.y * deltaTime * 60.0f;

        if (particles[i].position.y < 0)
        {
            particles[i].position.y = 475;
            particles[i].position.x = rand() % 700;
        }
    }
}

void RenderParticles()
{
    for (int i = 0; i < particles.size(); ++i)
    {
        ImGui::GetWindowDrawList()->AddCircleFilled(particles[i].position, particles[i].size, particles[i].color);
    }
}