#!/usr/bin/env bash
set -e
psql "user=postgres hostaddr=127.0.0.1 port=5432 password=postgres" < 40-drop-databases.sql
psql "user=postgres hostaddr=127.0.0.1 port=5432 password=postgres" < scripts/db-v1.sql
psql "user=postgres hostaddr=127.0.0.1 port=5432 password=postgres" < VersionScripts/v0001_0000.sql
psql "user=postgres hostaddr=127.0.0.1 port=5432 password=postgres" < 30-fill-test-DB.sql
