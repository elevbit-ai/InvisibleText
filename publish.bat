@echo off
cd /d "C:\Users\franc\OneDrive\READET~1\INVISI~1"
git branch -m main
git add .
git commit -m "InvisibleText v1.0 - by Joaquim Pedro de Morais Filho"
gh repo create InvisibleText --public --source=. --push
