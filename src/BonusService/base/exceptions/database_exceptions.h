#pragma once

#include "base_exception.h"

BADOON_EXCEPTION( DatabaseException, BaseException, "(Database error) " )
BADOON_EXCEPTION(
  DatabaseConnectException,
  DatabaseException,
  "(Database connection error) "
)
BADOON_EXCEPTION(
  DatabaseExecutionException,
  DatabaseException,
  "(Database execution error) "
)
