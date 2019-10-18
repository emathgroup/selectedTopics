import React from "react"
import { graphql } from "gatsby"
import Img from "gatsby-image"
const Imgdata = ({ data }) => {
  const { childImageSharp } = data.allFile.edges[0].node
  return <Img fixed={childImageSharp.fixed} />
}

export const query = graphql`
  query($id: String) {
    allFile(filter: { id: { eq: $id } }) {
      edges {
        node {
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
export default Imgdata
