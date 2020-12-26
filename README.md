# Dijkstra-s-Algorithm

## Notes

### Creating new Github Repository from VSCode:\
* STEP1: (VSCode) Open Folder -> Create and Select a New Folder -> Source Control -> Publish to Github
* STEP2: (GitHub) Create File Named ".gitignore" -> Select Template -> Commit
* GitHub Repo is now Linked With VSCode and Ready to be Used

### Using Valgrind for Memory Check:\
* STEP1: Download Ubuntu WSL from Microsoft Store
* STEP2: Install Necessary Commands
* STEP3: Navigate to Folder: `cd /mnt/c/...`
* STEP4: Compile the Project
* STEP5: `valgrind --leak-check=full --show-leak-kinds=all -s ./<executable_name> <argument_list>`