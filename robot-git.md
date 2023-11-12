Setup
=====

- For windows: from https://git-scm.com/download/win download and install Git-2.42.0.2-64-bit.exe
- For personal Linux: `sudo apt install git git-gui gitk tig`

Access to Epita gitlab
======================

- In a console (for windows a git console) `ssh-keygen -t ed25519 -f ~/.ssh/epita_gitlab_ed25519`
- In gitlab clic on user icon
- "Preferences"
- "SSH Keys"
- "Add new key"
- copy file contain ~/.ssh/epita_gitlab_ed25519.pub in gitlab
- "Add key"
- Edit ~/.ssh/config
- Add:
  Host gitlab.cri.epita.fr
    PreferredAuthentications publickey
    IdentityFile ~/.ssh/epita_gitlab_ed25519

Some git command
================

- Graphical tree display: `gitk --all . &`
- Graphical commit: `git gui &` (use check box next to commit message to amend commit in previous commit)
- Branch switch: `git checkout <branchName>`
- Interactive rebase:
  - `git branch sav`
  - `git rebase -i <previousCommitId>`
  - `git rebase --continue`
  - `git diff sav`
  - `git branch -D sav`
- Apply commit from other branch: `git cherry-pick <commitid>` + `git cherry-pick --continue`
- Delete previous commit (warning: all current modification and last commit are lost): `git reset --hard HEAD~1`
- official doc: https://git-scm.com/book/fr/v2/Utilitaires-Git-R%C3%A9%C3%A9crire-l%E2%80%99historique
- Video example:  [Git Rebase: Le rebasing interactif pour un historique propre by Felix Yadomi](https://www.youtube.com/watch?v=A_jreWjCl4s)

Git best practice
=================

- Do not commit generated file (build, managed_components, dependencies.lock, ...)
- Mark Generated files (or parent folder) in .gitignore
- One purpose by commit with a corresponding clear commit message
- Each commit compile
- To be able to review your commit and rewrite history easily:
  - You have to create dedicated branch for tp1
  - When you are ready for review, you have to create a merge request from tp1 to main
  - When you have my approval, you can merge your request
