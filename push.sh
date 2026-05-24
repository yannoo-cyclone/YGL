#!/bin/sh

# Vérifie s'il y a des changements à commiter
if [ -z "$(git status --porcelain)" ]; then
    echo "❌ Aucun changement à commiter."
    exit 1
fi

# Demande un message de commit (ou utilise un message par défaut)
read -p "Message de commit (ou appuyez sur Entrée pour 'Update') : " commit_msg
commit_msg=${commit_msg:-"Update"}

# Ajoute tous les fichiers modifiés/supprimés/ajoutés
git add .

# Commit
git commit -m "$commit_msg"

# Push vers origin/main
git push origin main

echo "✅ Commit et push réussis : '$commit_msg'"
