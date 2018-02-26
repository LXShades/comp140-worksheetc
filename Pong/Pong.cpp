#include "Pong.h"
#include <stdio.h>

bool Pong::Init(SDL_Renderer *renderer)
{
	controller.Init();

	ThePlayers[0].init();
	ThePlayers[0].SetSide(Player::Side_Left);
	ThePlayers[0].SetController(&controller);

	ThePlayers[1].init();
	ThePlayers[1].SetSide(Player::Side_Right);
	ThePlayers[1].SetController(&controller);

	TheBall.init();
	TheBall.SetController(&controller);

	Score[0] = 0;
	Score[1] = 0;

	debugPrinter = new DebugPrinter();
	debugPrinter->Init(renderer);
		
	return true;
}

bool Pong::Update()
{
	controller.UpdateIn();

	if(TheBall.IsOutOfBounds() == true)
	{
		Player::Side serveTo = Player::Side_Undefined;
		if(TheBall.OutOfBoundsSide() == Player::Side_Left)
		{
			Score[1]++;
			serveTo = Player::Side_Left;
		}

		if(TheBall.OutOfBoundsSide() == Player::Side_Right)
		{
			Score[0]++;
			serveTo = Player::Side_Right;
		}

		TheBall.init();
		TheBall.ServeTo(serveTo);

		// Flash the LEDs
		controller.ShowScores(2000, Score[0], Score[1]);

		// Reset the game if one of the players won
		if (Score[0] == 3 || Score[1] == 3)
		{
			Score[0] = Score[1] = 0;
		}
	}
	else
	{
		if( ((TheBall.IsColliding(ThePlayers[0]) == true) && (TheBall.IsGoingTo() == Player::Side_Left))
		  ||((TheBall.IsColliding(ThePlayers[1]) == true) && (TheBall.IsGoingTo() == Player::Side_Right))
		  )
		{
			TheBall.PlayerReturns();
		}

		ThePlayers[0].Move();
		ThePlayers[1].Move();
	}

	TheBall.Move();	

	// Output fading directional indicator lights
	float LeftOffset = ThePlayers[0].YToKnob(TheBall.GetPredictedLeftY() - ThePlayers[0].GetH() / 2) - ThePlayers[0].YToKnob(ThePlayers[0].GetY()),
		  RightOffset = ThePlayers[1].YToKnob(TheBall.GetPredictedRightY() - ThePlayers[1].GetH() / 2) - ThePlayers[1].YToKnob(ThePlayers[1].GetY());
	float LeftBlink = 1.0f, RightBlink = 1.0f;

	if (LeftOffset < 0)
	{
		controller.SetLEDBrightness(LED_TL, -LeftOffset);
		controller.SetLEDBrightness(LED_BL, 0.0f);
	}
	else
	{
		controller.SetLEDBrightness(LED_TL, 0.0f);
		controller.SetLEDBrightness(LED_BL, LeftOffset);
	}

	if (RightOffset < 0)
	{
		controller.SetLEDBrightness(LED_TR, -RightOffset);
		controller.SetLEDBrightness(LED_BR, 0.0f);
	}
	else
	{
		controller.SetLEDBrightness(LED_TR, 0.0f);
		controller.SetLEDBrightness(LED_BR, RightOffset);
	}

	// Blink lights if the ball is predicted to safely hit the centre
	if (abs(TheBall.GetPredictedLeftY() - (ThePlayers[0].GetY() + ThePlayers[0].GetH() / 2)) < ThePlayers[0].GetH() / 3)
	{
		float Lightness = ((SDL_GetTicks() / 17) & 1) ? 1.0f : 0.0f;
		controller.SetLEDBrightness(LED_TL, Lightness);
		controller.SetLEDBrightness(LED_BL, Lightness);
	}

	if (abs(TheBall.GetPredictedRightY() - (ThePlayers[1].GetY() + ThePlayers[1].GetH() / 2)) < ThePlayers[1].GetH() / 3)
	{
		float Lightness = !((SDL_GetTicks() / 17) & 1) ? 1.0f : 0.0f;
		controller.SetLEDBrightness(LED_TR, Lightness);
		controller.SetLEDBrightness(LED_BR, Lightness);
	}

	controller.UpdateOut();

	return true;
}

bool Pong::Draw(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

		SDL_Rect rect;

		rect.x = 0;
		rect.y = 0;
		rect.w = 800;
		rect.h = 600;
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, &rect);

		DrawCourt(renderer);

		ThePlayers[0].Render(renderer);
		ThePlayers[1].Render(renderer);

		TheBall.Render(renderer);

		
		for (int i = 0; i < TheBall.numDebugPoints; i++) {
			SDL_Rect point = {TheBall.debugPoints[i*2]-5, TheBall.debugPoints[i*2+1]-5, 10, 10};

			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			SDL_RenderFillRect(renderer, &point);
		}

		char str[255];
		sprintf(str,"%d - %d",Score[0],Score[1]);

		debugPrinter->SetScale(8, 8);
		debugPrinter->SetJustification(DebugPrinter::Justifiy_Centre);
		debugPrinter->Print(renderer, 400,20,RGBTOCOLOR(255,255,255),str);
		debugPrinter->SetScale(1, 1);
		debugPrinter->SetJustification(DebugPrinter::Justifiy_Left);
		
	
	return true;
}

bool Pong::Shutdown()
{
	controller.Shutdown();
	return true;
}



void Pong::DrawCourt(SDL_Renderer *renderer)
{	
	SDL_Rect rect;

	rect.x = 0;
	rect.y = 100;
	rect.w = 800;
	rect.h = 10;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect);

	rect.x = 0;
	rect.y = 590;
	rect.w = 800;
	rect.h = 10;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect);

	rect.x = 395;
	rect.y = 100;
	rect.w = 10;
	rect.h = 600;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect);

}