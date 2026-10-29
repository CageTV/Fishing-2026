Scriptname Fishing2026Native Hidden
{Functions of Fishing2026.dll}

; True when there is water at least afMinDepth game units deep (water surface above the ground) somewhere within afMaxDistance units in front of the actor.
; Replaces Fish Anywhere's projectile test, which failed at shallow shores. 70 units are about one metre.
bool Function IsWaterAhead(Actor akActor, float afMaxDistance = 900.0, float afMinDepth = 30.0) global native
