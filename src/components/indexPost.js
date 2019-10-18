import {graphql} from 'gatsby'
export default query = graphql`
fragment FrontmatterFragmentIndexBlog on MarkdownRemark{
    frontmatter{
        title
        date(formatString:"ddd, Do MMMM YYYY")
        tags
        thumbnail{
            childImageSharp{
                fixed(height:150 , width:150){
                    ...GatsbyImageSharpFixed
                }
            }
        }
    }
}
`