import React from "react"
import Layout from "../components/layout"
import "../styles/index.scss"
import {graphql} from 'gatsby'
import Img from 'gatsby-image'
const Test = ({data}) => {
    const {edges} = data.allFile;
  return (
    <Layout>
      <div>
        <h1>This is a test Page</h1>
        <p>
         {edges.map((edge)=>{
             const {id,childImageSharp} = edge.node;
             return(
                 <Img fixed={childImageSharp.fixed} key={id}/>
             )
         })}
        </p>
        
      </div>
    </Layout>
  )
}
export const query= graphql`
query{
    allFile(filter:{
        extension:{
            regex:"/(png|jpg|jpeg)/"
        }
    }){
        edges{
            node{
                id
                childImageSharp{
                    fixed(height:150 , width:150){
                        ...GatsbyImageSharpFixed
                    }    
                }
            }
        }
    }
}`
export default Test
