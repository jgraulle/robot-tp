Access to Epita gitlab
======================

- For windows: from https://git-scm.com/download/win download and install Git-2.42.0.2-64-bit.exe
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
