const config = {
    siteTitle: "数学研发论坛博客", // Site title.
    siteShortDesc: "Euler Wang Web developer",
    siteLogo: "/logos/logo-1024.png", // Logo used for SEO and manifest.
    siteUrl: "https://www.example.com", // Domain of your website without pathPrefix.
    siteDescription: "A GatsbyJS stater with Advanced design in mind.", // Website description used for RSS feeds/meta description tag.
    siteRss: "/rss.xml", // Path to the RSS file.
    googleAnalyticsID: "UA-47311644-5", // GA tracking ID.
    postDefaultCategoryID: "Tech", // Default category for posts.
    dateFromFormat: "YYYY-MM-DD", // Date format used in the frontmatter.
    dateFormat: "DD/MM/YYYY", // Date format for display.
    userName: "Euler Wang", // Username to display in the author segment.
    userEmail: "emath.ac.cn@gmail.com", // Email used for RSS feed's author segment
    userLocation: "India", // User location to display in the author segment.
    userAvatar: "", // User avatar to display in the author segment.
    userDescription: "Mathematics and Scientific Computing.", // User description to display in the author segment.
    // Links to social profiles/projects you want to display in the author segment/navigation bar.
    userLinks: [
      {
        label: "github",
        username: "emathgroup",
      },
      {
        label: "twitter",
        username: "test",
      },
      {
          label: "linkedIn",
          username: "hello",
      },
      {
        label: "qq",
        username: "6807814",
      },
      {
          label: "webchat",
          username: "rising-puffin",
      }
    ],
    copyright: "Copyright © 2019. Advanced User", // Copyright string for the footer of the website and RSS feed.
    themeColor: "#c62828", // Used for setting manifest and progress theme colors.
    backgroundColor: "#e0e0e0" // Used for setting manifest background color.
  };
  // Make sure siteUrl doesn't have an ending forward slash
if (config.siteUrl.substr(-1) === "/")
config.siteUrl = config.siteUrl.slice(0, -1);

// Make sure siteRss has a starting forward slash
if (config.siteRss && config.siteRss[0] !== "/")
config.siteRss = `/${config.siteRss}`;

module.exports = config;

  