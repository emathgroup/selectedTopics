# gatsby-react-blog
My Simple blog site with gatsby with react
## Installation


Run local Server:
```bash
npm install -g gatsby-cli gh-pages yarn
git clone https://github.com/emathgroup/selectedTopics
cd selectedTopics
yarn
# gatsby develop
gatsby build
gh-pages -r git@github.com:emathgroup/emathgroup.github.io.git -b master -d public

```
Go to `localhost:8000`
## Customization
- Put your markdown files in `/content/posts` and images in `content/images`
## Features
- Posts and pages in markdown
- tags
- Sorting by dates
- sass compiled
- code scheme (starring [New Moon](https://taniarascia.github.io/new-moon))
## Author
- Aman Kumar
