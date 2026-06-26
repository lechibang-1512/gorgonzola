# Git Submodule Troubleshooting Guide

When working with submodules (like `modules/gorgonzola-api-langs`), it's common to run into issues where the submodule shows up as "modified" in the parent repository's `git status`, even if you feel like you haven't changed anything.

This guide covers the two most common causes of this issue and how to prevent them.

## 1. Untracked Build Artifacts (`.pyc`, `__pycache__`, `build/`)

### The Problem
Git complains about "untracked content" or "modified content" when running tests or builds regenerates files (like Python's `.pyc` cache files). If these files are accidentally tracked by Git inside the submodule, they will show as modified every time they are regenerated. If they are untracked but not ignored, they will show up as untracked files.

### How to Prevent It
Always ensure that your submodule has a robust `.gitignore` file that ignores language-specific build artifacts. 

For Python, your `.gitignore` should always include:
```gitignore
__pycache__/
*.pyc
*.pyo
*.pyd
.Python
env/
venv/
build/
develop-eggs/
dist/
downloads/
eggs/
.eggs/
*.egg-info/
```

If files were accidentally tracked, you must remove them from the Git index (without deleting the local files) and commit the change:
```bash
git rm --cached -r __pycache__/
git commit -m "Stop tracking pycache files"
```

## 2. Parent-Submodule State Desync

### The Problem
A Git submodule is essentially a pointer to a specific commit in another repository. When you commit changes inside a submodule but fail to update the pointer in the parent repository (or if you undo the parent's commit using `git reset HEAD~`), the parent and submodule become desynchronized. 

The parent repository expects the submodule to be at `Commit A`, but the submodule's directory on your disk is actually checked out at `Commit B`. This causes the parent repository's `git status` to flag the submodule as `modified: (new commits)`.

### How to Prevent It
When making changes to a submodule, always follow this two-step commit process:

**Step 1: Commit and push inside the submodule**
```bash
cd modules/gorgonzola-api-langs
git add .
git commit -m "My submodule changes"
git push
```

**Step 2: Commit the new submodule reference in the parent**
```bash
cd ../../
git status # You will see the submodule is "modified"
git add modules/gorgonzola-api-langs
git commit -m "Update submodule reference"
git push
```

If you ever want to discard local desyncs and force your submodule to return to the exact commit the parent repository expects:
```bash
# This will discard all uncommitted changes in the submodule
git submodule update --init --recursive
```
