import React from 'react'
import Helmet from 'react-helmet'
import Layout from '../components/layout'
import  '../components/modules/posts.scss'
import { Link,graphql } from 'gatsby'
import Img from 'gatsby-image'

const tagPage = ({data,pageContext})=>{
    const { tag } = pageContext
    const { edges, totalCount } = data.allMarkdownRemark
    const tagHeader = `Post${
        totalCount === 1 ? "" : "s"
      } tagged with `
return(
    <Layout>
        <Helmet title={tagHeader}></Helmet>
        <h2 className={`title`}>{tagHeader}<u>{`${tag}`}</u></h2>
        <ul className={`posts`}>
            {edges.map(({node}) => {
                const {frontmatter,fields,id} = node
                const { title,date,thumbnail} = frontmatter
                const { slug } = fields
                return(
                    <li key={id} className={`post`}>
                        <Link className={`content_wrapper`} to ={`/blog/${slug}`} >
                        { thumbnail ? <Img fixed={thumbnail.childImageSharp.fixed}/> : null}
                        <div className={`content`}>
                        <h3>{title}</h3>
                        <p><span className={`date`}>{date}</span></p>
                        </div>
                        </Link>
                    </li>
                )
            })}
        </ul>
    </Layout>
)
}
export default tagPage
export const query = graphql`
query($tag : String){
    allMarkdownRemark(
        limit: 2000
        sort:{fields: [frontmatter___date], order: DESC }
        filter:{ frontmatter:{ tags: { in: [$tag] } } }
    ){
        totalCount
        edges{
            node{
                id,
                fields{
                    slug
                }
                ,
                ...FrontmatterFragmentBlog
            }
        }
    }
}
`
export const frontmatterfragment = graphql`
fragment FrontmatterFragmentBlog on MarkdownRemark{
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