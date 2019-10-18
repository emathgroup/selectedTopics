import React from "react"
import Layout from "../components/layout"
import "../styles/index.scss"
import { Link, graphql } from "gatsby"
import Img from "gatsby-image"
const Test = ({ data }) => {
  const { edges } = data.allFile
  return (
    <Layout>
      
        <h1>This is a test Page</h1>
        
          {edges.map(edge => {
            const { id, fields, childImageSharp } = edge.node
            const { slug } = fields
            return (
              <Link to={`/gallery/${slug}/${id}`} key={id}>
                <Img fixed={childImageSharp.fixed} />
              </Link>
            )
          })}
        
      
    </Layout>
  )
}
export const query = graphql`
  query($imgslug: String) {
    allFile(
      filter: {
        extension: { regex: "/(png|jpg|jpeg)/" }
        fields: { slug: { eq: $imgslug, ne: null } }
      }
    ) {
      edges {
        node {
          id
          fields {
            slug
          }
          childImageSharp {
            fixed(height: 150, width: 150) {
              ...GatsbyImageSharpFixed
            }
          }
        }
      }
    }
  }
`
export default Test
