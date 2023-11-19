#pragma once

#include "base_exception.h"

BADOON_EXCEPTION( LogicException, BaseException, "(Logic error)" )
BADOON_EXCEPTION(
  InvalidAuthDataException,
  LogicException,
  "(Invalid auth data)"
)
BADOON_EXCEPTION(
  RecomendationStrategyException,
  LogicException,
  "(Recomendation Strategy)"
)
BADOON_EXCEPTION(
  CritertionException,
  RecomendationStrategyException,
  "(Criterion)"
)
