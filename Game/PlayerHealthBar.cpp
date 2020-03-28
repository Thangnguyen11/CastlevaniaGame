#include "PlayerHealthBar.h"

PlayerHealthBar::PlayerHealthBar(int PlayerHealth, float X, float Y)
{
	currentPlayerHealth = PlayerHealth;
	maxPlayerHealth = PlayerHealth;
	posX = X;
	posY = Y;

	for (int i = 0; i < currentPlayerHealth; i++) {
		Texture2d* TextureStackHealth = Texture2dManager::GetInstance()->GetTexture(EntityType::PLAYERHP);
		Sprite* spriteStackHealth = new Sprite(TextureStackHealth, MaxFrameRate);
		health.push_back(spriteStackHealth);
	}
}

PlayerHealthBar::~PlayerHealthBar() {}

void PlayerHealthBar::Update(int currentHealth, float X, float Y)
{
	currentPlayerHealth = currentHealth;
	posX = X;
	posY = Y;
}

void PlayerHealthBar::Render()
{
	if (currentPlayerHealth <= 0)
		return;
	else
		for (int i = 0; i < maxPlayerHealth; i++) {
			for (int j = 0; j < currentPlayerHealth; j++) {
				health[j]->SelectFrame(HEALTH_STATE_HAVE);
				health[j]->Draw(posX + i * 10, posY + 100);
			}
			for (int l = currentPlayerHealth; l < maxPlayerHealth; l++) {
				health[l]->SelectFrame(HEALTH_STATE_LOST);
				health[l]->Draw(posX + i * 10, posY);
			}
		}
}
