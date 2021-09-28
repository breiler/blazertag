#ifndef __GAMEEVENTTYPE_H
#define __GAMEEVENTTYPE_H

enum GameEventType
{
    /**
     * When game was properly initialized
     */
    GAME_INITALIZED,

    /**
     * When the player was hit and lost a life
     */
    HIT,

    /**
     * When the player was hit and the last life was used
     */
    HIT_KILLED,

    /**
     * When the player is hit when dead. 
     */
    HIT_DEAD,

    /**
     * When a shot was fired
     */
    FIRING_SHOT,

    /**
     * When a shot was attempted and there was no more ammo
     */
    FIRING_SHOT_NO_AMMO,

    /**
     * When a shot was attempted and the player is dead
     */
    FIRING_SHOT_NO_HEALTH,

    /**
     * When a shot was attempted but the gun was still cooling down
     */
    FIRING_SHOT_COOLING_DOWN,

    /**
     * A game update has occured
     */
    GAME_TICK,

    /**
     * Game over
     */
    GAME_OVER
};

#endif