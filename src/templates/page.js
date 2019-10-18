import React from "react"
import Layout from "../components/layout"
import { graphql, Link } from "gatsby"
import "../components/modules/post.scss"
import Helmet from "react-helmet"
import Img from "gatsby-image"
import "../styles/page.scss"
export const query = graphql`
  query($slug: String) {
    markdownRemark(fields: { slug: { eq: $slug } }) {
      ...FrontmatterFragmentBlog
      html
      fields {
        slug
      }
    }
  }
`
const Blog = props => {
  const { frontmatter, html, fields } = props.data.markdownRemark
  const { title, date, tags, thumbnail } = frontmatter
  const { slug } = fields
  const gitlink =
    "https://github.com/emathgroup/selectedTopics/blob/master/content/posts/"
  return (
    <Layout>
      <Helmet title={title} />
      <div className="head_container">
        {thumbnail ? <Img fixed={thumbnail.childImageSharp.fixed} /> : null}
        <div className={`head_wrapper`}>
          <h2 className={`head_title`}>{title}</h2>
          <p>
            <span className={`date`}>{date}</span>
            <span className="gitlink">
              <a
                className="link"
                rel="noopener noreferrer"
                target="_blank"
                href={`${gitlink}/${slug}.md`}
              >
                Edit on Github
              </a>
            </span>
            {tags.map((tag, index) => {
              return (
                <Link className={`link`} to={`/tags/${tag}`} key={index}>
                  <span className={`tag`}>{tag}</span>
                </Link>
              )
            })}
          </p>
        </div>
      </div>
      <div dangerouslySetInnerHTML={{ __html: html }}></div>
    </Layout>
  )
}
export default Blog
