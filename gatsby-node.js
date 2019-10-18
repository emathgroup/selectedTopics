const path = require("path")
// const _ = require("lodash")
module.exports.onCreateNode = ({ node, actions }) => {
  const { createNodeField } = actions
  // node.internal.mediaType == "image/jpeg" || node.internal.mediaType == "image/png"  ? console.log('@@@@@',node) : null;
  if (node.internal.type == "MarkdownRemark") {
    const slug = path.basename(node.fileAbsolutePath, ".md")
    createNodeField({
      node,
      name: "slug",
      value: slug,
    })
  }
  if (
    /image\/*/.test(node.internal.mediaType) &&
    /thumbnails/.test(node.relativeDirectory)
  ) {
    // console.log("@@@@@", node)
    const imgSlug = node.relativeDirectory
    createNodeField({
      node,
      name: "slug",
      value: imgSlug,
    })
  }
}
module.exports.createPages = async ({ graphql, actions }) => {
  const { createPage } = actions
  const tagTemplate = path.resolve(`src/templates/tags.js`)
  const blogTemplate = path.resolve(`src/templates/page.js`)
  const imgTemplate = path.resolve(`src/templates/img.js`)
  const imgFulltemplate = path.resolve(`src/templates/img-template.js`)
  const res = await graphql(`
    query {
      allFile(filter: { fields: { slug: { ne: null } } }) {
        edges {
          node {
            id
            fields {
              slug
            }
          }
        }
      }
      allMarkdownRemark(filter: { fields: { draft: { eq: false } } }) {
        edges {
          node {
            id
            fields {
              slug
            }
            frontmatter {
              tags
            }
          }
        }
      }
    }
  `)

  tagSet = new Set()
  imgSet = new Set()
  const posts = res.data.allMarkdownRemark.edges
  posts.forEach(post => {
    const { slug } = post.node.fields
    const { tags } = post.node.frontmatter
    createPage({
      component: blogTemplate,
      path: `/blog/${slug}`,
      context: {
        slug,
      },
    })
    if (tags) {
      tags.forEach(tag => {
        tagSet.add(tag)
      })
    }
    // }
  })

  tagArray = Array.from(tagSet)
  tagArray.forEach(tag => {
    createPage({
      component: tagTemplate,
      path: `/tags/${tag}`,
      context: {
        tag,
      },
    })
  })
  const imgSlug = res.data.allFile.edges
  //   console.log(imgSlug);
  imgSlug.forEach(edge => {
    const { fields, id } = edge.node
    const { slug } = fields
    imgSet.add(slug)
    createPage({
      component: imgFulltemplate,
      path: `/gallery/${slug}/${id}`,
      context: {
        id,
      },
    })
  })
  imgSlugArray = Array.from(imgSet)
  imgSlugArray.forEach(slug => {
    createPage({
      component: imgTemplate,
      path: `/gallery/${slug}`,
      context: {
        imgslug: slug,
      },
    })
  })
}
