@echo off
setlocal enabledelayedexpansion
for /L %%x in (1, 1, 50) do (
  mkdir newdir%%x
  cd newdir%%x
)
endlocal
