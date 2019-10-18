/**
 * Configure your Gatsby site with this file.
 *
 * See: https://www.gatsbyjs.org/docs/gatsby-config/
 */
const config = require("./data/siteConfig")
const [github, twitter, linkedin] = config.userLinks
module.exports = {
  /* Your site config here */
  siteMetadata: {
    title: config.siteTitle,
    author: config.userName,
    Email: config.userEmail,
    githubUsername: github.username,
    twitterUsername: twitter.username,
    linkedInUsername: linkedin.username,
    defaultTitle: config.siteTitle,
    defaultDescription: config.siteDescription,
    siteUrl: config.siteUrl,
    defaultImage: config.siteLogo,
  },
  plugins: [
    "gatsby-plugin-sass",
    {
      resolve: `gatsby-source-filesystem`,
      options: {
        path: `${__dirname}/content/`,
        name: `content`,
      },
    },
    {
      resolve: `gatsby-source-mongodb`,
      options: {
        connectionString:
          `mongodb+srv://aman:${process.env.PASSWORD}@cluster0-pe0lk.mongodb.net/test?retryWrites=true&w=majority`,
        extraParams: {
          replicaSet: "cluster0-shard-0",
          ssl: true,
          authSource: "admin",
        },
        dbName: "ace",
        collection: ["People","team_2018-19"],
        clientOptions: {
          useNewUrlParser: true,
        },
      },
    },
    {
      resolve: `gatsby-plugin-typography`,
      options: {
        pathToConfigModule: `src/utils/typography`,
      },
    },
    "gatsby-plugin-draft",
    {
      resolve: `gatsby-transformer-remark`,
      options: {
        plugins: [
          "gatsby-remark-relative-images",
          "gatsby-remark-prismjs",
          "gatsby-remark-copy-linked-files",
          {
            resolve: `gatsby-remark-images`,
            options: {
              maxWidth: 750,
              linkImagesToOriginal: false,
            },
          },
          {
            resolve: `gatsby-remark-katex`,
            options: {
              // Add any KaTeX options from https://github.com/KaTeX/KaTeX/blob/master/docs/options.md here
              strict: "ignore",
              displayMode:true,
              throwOnError:false,
              errorColor:"#cc0000",
            }
          }
        ],
      },
    },
    "gatsby-plugin-sharp",
    `gatsby-transformer-sharp`,
  ],
}
