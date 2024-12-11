import { defineConfig } from 'vitepress'
import { withMermaid } from 'vitepress-plugin-mermaid' 

// https://vitepress.dev/reference/site-config
let config = defineConfig({
  base: "/bilive/",
  title: "bilive",
  description: "Official documentation",
  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    nav: [
      { text: 'Home', link: '/' },
      { text: 'Guide', link: '/getting-started' }
    ],
    sidebar: [
      {
        text: 'Guide',
        items: [
          { text: 'Getting Started', link: '/getting-started' },
          { text: 'Test Hardware', link: '/test-hardware' },
          { text: 'Installation & Usage', link: '/installation' },
          { text: 'Models', link: '/models' },
          { text: 'Questions', link: '/install-questions' },
          { text: 'Biliup', link: '/biliup' },
          { text: 'Record', link: '/record' },
          { text: 'Upload', link: '/upload' },
          { text: 'Scan', link: '/scan' },
          { text: 'Reference', link: '/reference' }
        ]
      }
    ],
    outline: {
      level: [2, 4]
    },
    socialLinks: [
      { icon: 'github', link: 'https://github.com/timerring/bilive' }
    ],
  },
    // optionally, you can pass MermaidConfig
    mermaid: {
      // refer for options:
      // https://mermaid.js.org/config/setup/modules/mermaidAPI.html#mermaidapi-configuration-defaults
    },
    // optionally set additional config for plugin itself with MermaidPluginConfig
    mermaidPlugin: {
      // set additional css class for mermaid container
      class: "mermaid"
    }
})

config = withMermaid(config) 

export default config

