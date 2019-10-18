import React from 'react'
import {  StaticQuery,graphql} from 'gatsby'
import Layout from '../components/layout'
import Helmet from 'react-helmet'
import config from '../../data/siteConfig'
const Aboutpage = () =>{
    const query = graphql`
    query{
        site{
            siteMetadata{
                githubUsername
                twitterUsername
                linkedInUsername
            }
        }
    }
    `
    return(
        <Layout>
            <Helmet title={`Me - ${config.userName}`}/>
        <div>
            <h2>About me</h2>
            <p>Hi ! I am <strong>Aman Kumar</strong> a Web enthusiast and Junior year student at IIT Patna. I like to spend my leisure time working on web projects.</p>
            <p>I started this website as a place to document everything i learned during development throughout internet. I write about stuff in very short and crisp manner.</p>
            <br/>
            <h3 id="around-the-web">Around the web</h3><br/>
            <StaticQuery query={query} render={
                ({site:{
                    siteMetadata:{
                        githubUsername:github,
                        twitterUsername:twitter,
                        linkedInUsername:linkedin
                    }
                }})=>{
                    return(
                        <ul>
                            <li>Github: <a href={`https://github.com/${github}`}>{github}</a></li>
                            <li>Twitter: <a href={`https://twitter.com/${twitter}`}>{twitter}</a></li>
                            <li>LinkedIn: <a href={`https://linkedin.com/in/${linkedin}`}>{linkedin}</a></li>
                        </ul>
                    )
                }
            } />
        </div>
        <div>
            <h2 id="currently-using">Currently Using</h2>
            <hr/>
            <ul>
                <li>Computer: Dell Inspiron 5557</li>
                <li>Hosting: <a href={`https://netlify.com`}>Netlify</a></li>
                <li>Editor: <a href={`https://visualstudio.com`}>Visual Studio Code</a></li>
                <li>Static Site Generator: <a href={`https://gatsbyjs.org`}>The Great GatsbyJs</a></li>
                <li>Syntax Highlighting: <a href={`https://prismjs.com`}>PrismJs</a></li>
                <li>Code syntax theme: <a href={`https://taniarascia.github.io/new-moon`}>New Moon</a></li>
                <li>Website theme: <a href={`https://taniarascia.com`}>Tania Rascia</a></li>
            </ul>
        </div>
        </Layout>
        
    )
}
export default Aboutpage